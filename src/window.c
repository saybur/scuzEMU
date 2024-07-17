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
#include "list.h"
#include "window.h"
#include "util.h"

#define WINDOW_WIDTH        261
#define WINDOW_HEADER       46

static WindowPtr window;
static ListHandle list;
static Handle icon_device, icon_files, icon_images;
static Str31 note;
static Str15 str_device, str_files, str_images;
static short content_type;

/**
 * Draws the window reminder/note text.
 *
 * @param erase  if true, erase the text area prior to drawing.
 */
static void window_draw_note(Boolean erase)
{
	Rect bounds;

	bounds = window->portRect;
	SetRect(&bounds,
			bounds.left + 10,
			bounds.top + 32,
			bounds.left + (WINDOW_WIDTH - 70),
			bounds.top + 44);

	if (erase) {
		EraseRect(&bounds);
	}

	MoveTo(bounds.left + 2, bounds.bottom - 2);
	TextSize(10);
	DrawString(note);
	TextSize(0);
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
	Rect r;

	GetPort(&old_port);
	SetPort(window);

	EraseRect(&(window->portRect));

	MoveTo(0, WINDOW_HEADER - 1);
	LineTo(WINDOW_WIDTH, WINDOW_HEADER - 1);

	if (icon_device) {
		SetRect(&r, 10, 10, 26, 26);
		PlotIcon(&r, icon_device);
	}

	MoveTo(36, 22);
	TextFont(0);
	TextSize(12);
	DrawString(str_device);

	MoveTo(156, 22);
	if (content_type) {
		if (icon_images) {
			SetRect(&r, 130, 10, 146, 26);
			PlotIcon(&r, icon_images);
		}
		DrawString(str_images);
	} else {
		if (icon_files) {
			SetRect(&r, 130, 10, 146, 26);
			PlotIcon(&r, icon_files);
		}
		DrawString(str_files);
	}
	TextFont(1);
	TextSize(0);

	window_draw_note(false);

	UpdateControls(window, window->visRgn);
	LUpdate(window->visRgn, list);
	list_draw_grow(window, list);

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
	list_vis.top = list_vis.top + WINDOW_HEADER;
	list_vis.left = list_vis.left;
	list_vis.right = list_vis.right - 15;

	SetRect(&list_con, 0, 0, 1, 0);
	SetPt(&list_cell, 0, 0);

	list = LNew(&list_vis, &list_con, list_cell, 0, window, true, true, false, true);
	list_size(list, -1, -1);

	icon_device = GetIcon(ICON_DEVICE);
	icon_files = GetIcon(ICON_FILES);
	icon_images = GetIcon(ICON_IMAGES);

	str_load(STR_GENERAL, STRI_GEN_HEAD_DEV, str_device, 16);
	str_load(STR_GENERAL, STRI_GEN_HEAD_FILE, str_files, 16);
	str_load(STR_GENERAL, STRI_GEN_HEAD_IMG, str_images, 16);

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
	list_draw_grow(window, list);
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
	/* delegate everything to the list */
	list_key(list, evt);
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
	limits.left = WINDOW_WIDTH;
	limits.right = WINDOW_WIDTH;
	limits.top = WINDOW_MIN_HEIGHT;

	if (nsize = GrowWindow(window, p, &limits)) {
		SizeWindow(window, LoWord(nsize), HiWord(nsize), true);
		list_size(list, LoWord(nsize), HiWord(nsize) - WINDOW_HEADER);
		InvalRect(&(window->portRect));
	}

	SetPort(old_port);
}

/**
 * Provides the next selected item number within the list. This calls through
 * to list_next(), see that for details.
 *
 * @param *i  on input, the cell to start at; will be set to the next selected
 *            cell, or -1 if there are no more cells.
 */
void window_next(short *i)
{
	list_next(list, i);
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
 * @param scsi  the SCSI ID being assigned, on [0, 6]
 * @param mode  the kind of data being assigned, per above.
 * @param h     block of data to parse in emu_populate_list().
 * @param len   length of above data.
 * @return      the number of entries found in the list.
 */
short window_populate(short scsi, short mode, Handle h, short len)
{
	short num;

	if (scsi < 0) scsi = 0;
	if (scsi > 6) scsi = 6;
	if (str_device[0] > 0) {
		str_device[str_device[0]] = '0' + scsi;
	}

	num = emu_populate_list(list, h, len);
	content_type = mode;
	HLockHi((Handle) list);
	if (mode == 0) {
		(*list)->selFlags = lUseSense | lNoRect | lNoExtend;
	} else {
		(*list)->selFlags = lOnlyOne;
	}
	HUnlock((Handle) list);
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
 * Updates the informational text note at the top of the window.
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
		BlockMove(str, note, len + 1);
	} else {
		/* use default */
		if (content_type) {
			str_load(STR_GENERAL, STRI_GEN_IMAGES, note, 32);
		} else {
			str_load(STR_GENERAL, STRI_GEN_FILES, note, 32);
		}
	}

	GetPort(&old_port);
	SetPort(window);

	window_draw_note(true);

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
