/*
 * Copyright (C) 2024 saybur
 *
 * This program is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with this
 * program. If not, see <https://www.gnu.org/licenses/>.
 */

#include "config.h"
#include "constants.h"
#include "emu.h"
#include "window.h"
#include "util.h"

static WindowPtr window;
static ListHandle list;
static Str31 text;
static short content_type;

static Str15 search;
static long last_search;

/**
 * Callback for LSearch that implements the common form of "find the first thing that
 * matches this random text string" rather than a full-text match.
 */
pascal short list_cmp(char *cell, char *test, short cl, short tl)
{
	short len, i;
	Boolean match;

	len = cl;
	if (tl < cl) len = tl;

	match = true;
	for (i = 0; i < len; i++) {
		if (lowerc(cell[i]) != lowerc(test[i])) {
			match = false;
			break;
		}
	}

	/* needs inverted logic */
	if (match) {
		return 0;
	} else {
		return 1;
	}
}

/**
 * Draws the window.
 *
 * This implementation is pretty naive and just draws the whole window, which is OK
 * (I guess) for such a simple application but should probably be improved in future
 * releases.
 */
static void window_draw(void)
{
	GrafPtr old_port;
	Rect bounds;

	GetPort(&old_port);
	SetPort(window);

	EraseRect(&(window->portRect));

	UpdateControls(window, window->visRgn);
	LUpdate(window->visRgn, list);
	DrawGrowIcon(window);

	bounds = window->portRect;
	MoveTo(bounds.left + 3, bounds.bottom - 3);
	TextSize(10);
	DrawString(text);
	TextSize(0);

	SetPort(old_port);
}

/**
 * Called during program startup to build window contents. This implementation uses only
 * one window, so we can 'cheat' and use this one window (and its memory) throughout the
 * program life. It also avoids the need to pass around WindowPtr references.
 *
 * The window will still be hidden after this call.
 *
 * @return  true if success, false if failed due to lack of memory.
 */
Boolean window_init(void)
{
	Rect list_vis, list_con;
	Point list_cell;
	short i;
	Point p;

	if (g_use_qdcolor) {
		window = GetNewCWindow(WIND_MAIN, 0, (WindowPtr)-1);
	} else {
		window = GetNewWindow(WIND_MAIN, 0, (WindowPtr)-1);
	}
	if (!window) {
		return false;
	}
	SetWRefCon(window, WIND_MAIN);

	SetPort(window);

	list_vis = window->portRect;
	list_vis.right = list_vis.right - 15;
	list_vis.bottom = list_vis.bottom - 15;

	SetRect(&list_con, 0, 0, 1, 0);
	SetPt(&list_cell, 0, 0);

	list = LNew(&list_vis, &list_con, list_cell, 0, window, true, true, false, true);
	HLock((Handle) list);

	search[0] = 0;
	last_search = 0;

	content_type = -1;
	return true;
}

/**
 * Handles /activateEvt/.
 *
 * @param active  true if activating, false if inactivating.
 */
void window_activate(Boolean active)
{
	SetPort(window);

	LActivate(active, list);
	DrawGrowIcon(window);
}

/**
 * Handles /inContent/ on /mouseDown/.
 *
 * @param evt     the event record causing the click.
 * @param dclick  true if there was a list double-click.
 */
void window_click(EventRecord *evt, Boolean *dclick)
{
	GrafPtr old_port;

	GetPort(&old_port);
	SetPort(window);
	GlobalToLocal(&(evt->where));

	*dclick = LClick(evt->where, evt->modifiers, list);

	SetPort(old_port);
}

/**
 * Gets an item name from the internal list and returns it in the given string.
 * To protect against future emulator API updates, ensure the string has at least
 * 64 total bytes of storage.
 *
 * @param item  the item number (i.e. list vertical index) to fetch.
 * @param str   address of string to store the Pascal-style result in.
 */
void window_get_item_name(short item, unsigned char *str)
{
	Point p;
	short l;

	SetPt(&p, 0, item);
	l = 63;
	LGetCell(&(str[1]), &l, p, list);
	str[0] = l & 0xFF;
}

/**
 * Handles /inContent/ on /keyDown/.
 *
 * @param evt  the event record causing the click.
 */
void window_key(EventRecord *evt)
{
	char c;
	short count;
	long t;
	Point p, op;
	short is_shift;

	is_shift = evt->modifiers & shiftKey;
	/* hack: if list doesn't allow multiple selection pretend shift is off */
	if ((*list)->selFlags & lOnlyOne) {
		is_shift = 0;
	}

	c = evt->message & 0xFF;
	if (c >= 0x20) {
		/* interpret as character */

		/* add to existing text search or not? */
		t = TickCount();
		if (t - last_search > LIST_TYPING_TIME) {
			search[0] = 1;
			search[1] = c;
			last_search = t;
		} else if (search[0] >= 14) {
			/* no room, ignore; fast typist! (or coding error) */
		} else {
			last_search = t;
			search[++search[0]] = c;
		}

		/* find cell with this name */
		SetPt(&p, 0, 0);
		if (LSearch(&(search[1]), search[0], list_cmp, &p, list)) {
			/* deselect old cells */
			list_clear_selections(list);

			/* select the new cell */
			LSetSelect(true, p, list);
			LAutoScroll(list);
		}
	} else if (c >= 0x1C) {
		/* arrow keys */

		count = emu_get_count();
		SetPt(&p, 0, 0);
		if (LGetSelect(true, &p, list)) {
			/* existing cell is selected, start from there */
			if (! is_shift) {
				list_clear_selections(list);
			}

			/* shift to new cell position and select it (with roll-over) */
			if (c & 1) {
				/* right/down */
				if (is_shift) {
					/* move to last selected cell before expanding */
					while (LGetSelect(true, &p, list)) {
						p.v++;
					}
				} else {
					p.v++;
				}
				if (p.v >= count) {
					if (is_shift) {
						/* avoid wrapping, it feels weird */
						p.v = count - 1;
					} else {
						p.v = 0;
					}
				}
			} else {
				/* left/up */
				p.v--;
				if (p.v < 0) {
					if (is_shift) {
						p.v = 0;
					} else {
						p.v = count - 1;
					}
				}
			}

			/* if legal, select cell */
			if (p.v >= 0 && p.v < count) {
				LSetSelect(true, p, list);
			}
		} else {
			/* just select the first list entry */
			LSetSelect(true, p, list);
		}
		LAutoScroll(list);
	} else if (c == 0x1B) {
		/* esc/clear */
		list_clear_selections(list);
	}
}

/**
 * Handles /inGrow/ on /mouseDown/.
 *
 * @param p  the point to grow/shrink the window to.
 */
void window_grow(Point p)
{
	GrafPtr old_port;
	long nsize;
	Rect limits;

	GetPort(&old_port);
	SetPort(window);

	limits = (*GetGrayRgn())->rgnBBox;
	limits.left = WINDOW_MIN_WIDTH;
	limits.top = WINDOW_MIN_HEIGHT;

	if (nsize = GrowWindow(window, p, &limits)) {
		SizeWindow(window, LoWord(nsize), HiWord(nsize), true);
		LSize(LoWord(nsize) - 15, HiWord(nsize) - 15, list);
		EraseRect(&(window->portRect));
		InvalRect(&(window->portRect));
	}

	SetPort(old_port);
}

/**
 * Provides the next selected item number within the list.
 *
 * This is just a wrapper around LGetSelect(true), so it can be called repeatedly,
 * starting at 0, returning the next selected cell each time (though don't forget
 * to add 1 to the result to move past the last-selected cell).
 *
 * @param *i  on input, the cell to start at; will be set to the next selected
 *            cell, or -1 if there are no more cells.
 */
void window_next(short *i)
{
	Point p;

	p.h = 0;
	p.v = *i;
	if (LGetSelect(true, &p, list)) {
		*i = p.v;
	} else {
		*i = -1;
	}
}

/**
 * Sets the window contents to the given block of memory.
 *
 * Most of the heavy lifting is passed off to emu.c, but this still updates some internal
 * references to assist in drawing the window.
 *
 * The mode below defines how the window is drawn for the contents, where 0 is files, 1 is
 * images, and everything else is "no content."
 *
 * @param mode  the kind of data being assigned, per above.
 * @param h     block of data to parse in emu_populate_list().
 * @param len   length of above data.
 * @return      the number of entries found in the list.
 */
short window_populate(short mode, Handle h, short len)
{
	short num;

	num = emu_populate_list(list, h, len);
	content_type = mode;
	if (mode == 0) {
		(*list)->selFlags = lUseSense | lNoRect | lNoExtend;
	} else {
		(*list)->selFlags = lOnlyOne;
	}
	window_text(0);
	return num;
}

/**
 * Handles /osEvt/.
 *
 * @param resume true if resuming, false if suspending.
 */
void window_resume(Boolean resume)
{
	if (resume) {
		SelectWindow(window);
	}
	HiliteWindow(window, resume);
	window_activate(resume);
}

/**
 * Shows or hides the single program window, without doing disposal.
 *
 * @param open true to show, false to hide.
 */
void window_show(Boolean open)
{
	if (open) {
		ShowWindow(window);
		SelectWindow(window);
	} else {
		HideWindow(window);
	}
}

/**
 * Updates the informational text at the bottom of the window.
 *
 * Needs a Pascal-style string. This will set the UI to match. If the provided
 * pointer is NIL it will set the text to default.
 *
 * @param str  the new bottom window text.
 */
void window_text(unsigned char *str)
{
	unsigned char len;
	GrafPtr old_port;
	Rect bounds;

	if (str) {
		/* copy string to local; don't forget length in Pascal string */
		len = (unsigned char) str[0];
		if (len > 31) len = 31;
		BlockMove(str, text, len + 1);
	} else {
		/* use default */
		if (content_type) {
			str_load(STR_GENERAL, STRI_GEN_IMAGES, text, 32);
		} else {
			str_load(STR_GENERAL, STRI_GEN_FILES, text, 32);
		}
	}

	GetPort(&old_port);
	SetPort(window);

	/* clear text already present in UI */
	bounds = window->portRect;
	SetRect(&bounds,
			bounds.left + 1,
			bounds.bottom - 14,
			bounds.left + (WINDOW_MIN_WIDTH - 22),
			bounds.bottom - 1);
	EraseRect(&bounds);

	/* draw the new text */
	MoveTo(bounds.left + 2, bounds.bottom - 2);
	TextSize(10);
	DrawString(text);
	TextSize(0);

	SetPort(old_port);
}

/**
 * Handles /updateEvt/.
 */
void window_update(void)
{
	BeginUpdate(window);
	window_draw();
	EndUpdate(window);
}
