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

#include "constants.h"
#include "window.h"

static WindowPtr window;
static ListHandle list;
static Str255 text;
static short content_type;

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

	window = GetNewWindow(WIND_MAIN, 0, (WindowPtr)-1);
	if (!window) {
		return false;
	}

	SetPort(window);

	list_vis = window->portRect;
	list_vis.right = list_vis.right - 15;
	list_vis.bottom = list_vis.bottom - 15;

	SetRect(&list_con, 0, 0, 1, 0);
	SetPt(&list_cell, 0, 0);

	list = LNew(&list_vis, &list_con, list_cell, 0, window, true, true, false, true);
	if (list) {
		(*list)->selFlags = lNoRect;
	} else {
		return false;
	}

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
 * This is narrowly tailored to mouse clicks on the list and will need expansion
 * if more program features are added.
 *
 * @param evt  the event record causing the click.
 * @param i    the cell item vertical index clicked on, if applicable.
 * @return     true if a cell was selected, false otherwise.
 */
Boolean window_click(EventRecord *evt, short *i)
{
	Boolean double_click;
	Cell selected_cell;
	Str255 s;

	GlobalToLocal(&(evt->where));
	double_click = LClick(evt->where, evt->modifiers, list);
	if (double_click) {

		SetPt(&selected_cell, 0, 0);
		if (LGetSelect(true, &selected_cell, list)) {
			*i = selected_cell.v;
			return true;
		}

	}

	return false;
}

/**
 * Gets an item name from the internal list and returns it in the given string.
 *
 * @param item  the item number (i.e. list vertical index) to fetch.
 * @param str   address of string to store the result in.
 */
void window_get_item_name(short item, Str255 str)
{
	Point p;
	short l;

	SetPt(&p, 0, item);
	l = 256;
	LGetCell(&(str[1]), &l, p, list);
	str[0] = l & 0xFF;
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
 */
void window_populate(short mode, Handle h, short len)
{
	emu_populate_list(list, h, len);
	content_type = mode;
	window_text(0);
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
		BlockMove(str, text, len + 1);
	} else {
		/* use default */
		if (content_type) {
			GetIndString(text, STR_GENERAL, STRI_GEN_IMAGES);
		} else {
			GetIndString(text, STR_GENERAL, STRI_GEN_FILES);
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
