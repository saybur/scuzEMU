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

#include "util.h"

static void (*quit_func)(void);
static Cursor busy_curs;

/**
 * Presents an alert using the given ALRT resource, with a message contained in a
 * STR# resource with a matching ID.
 *
 * This provides a simple way to create alert templates without having to build a
 * dedicated ALRT. The "^0" will be replaced with the string at the given index in
 * the STR# resource.
 *
 * @param type    type of alert to use, 0 for default (StopAlert).
 * @param res_id  the ALRT and STR# resources to pull from.
 * @param str_id  the string ID within the resource to get.
 */
void alert_template(short type, short res_id, short str_id)
{
	unsigned char *s;

	if (! (s = (unsigned char *) NewPtr(256))) {
		mem_fail();
	}

	/* load the error message */
	GetIndString(s, res_id, str_id);

	/* show message */
	SetCursor(&arrow);
	ParamText(s, 0, 0, 0);
	switch (type)
	{
	case ATYPE_CAUTION:
		CautionAlert(res_id, 0);
		break;
	case ATYPE_NOTE:
		NoteAlert(res_id, 0);
		break;
	default:
		StopAlert(res_id, 0);
	}

	DisposPtr((Ptr) s);
}

/**
 * Presents an alert using the given ALRT resource, with a message contained in a
 * STR# resource with a matching ID, and a given error code.
 *
 * This is the same as the above call, except that "^1" will be replaced by a
 * rendered error code.
 *
 * @param type    type of alert to use, 0 for default (StopAlert).
 * @param res_id  the ALRT and STR# resources to pull from.
 * @param str_id  the string ID within the resource to get.
 * @param err     the error code to render.
 */
void alert_template_error(short type, short res_id, short str_id, short err)
{
	unsigned char *s;
	Str15 is;

	if (! (s = (unsigned char *) NewPtr(256))) {
		mem_fail();
	}

	/* load the error message */
	GetIndString(s, res_id, str_id);

	/* render error num surround with " (num)"; string size should be OK */
	NumToString(err, &(is[2]));
	is[0] = is[2] + 3;
	is[1] = ' ';
	is[2] = '(';
	is[is[0]] = ')';

	/* show message */
	SetCursor(&arrow);
	ParamText(s, is, 0, 0);
	switch (type)
	{
	case ATYPE_CAUTION:
		CautionAlert(res_id, 0);
		break;
	case ATYPE_NOTE:
		NoteAlert(res_id, 0);
		break;
	default:
		StopAlert(res_id, 0);
	}

	DisposPtr((Ptr) s);
}

/**
 * Deletes an item from an array of shorts, shifting remaining entries.
 *
 * @param arr  an array of shorts.
 * @param len  the length of the array.
 * @param itm  the item to remove.
 */
void arr_del_short(short *arr, short len, short itm)
{
	short i;

	for (i = itm + 1; i < len; i++) {
		arr[i - 1] = arr[i];
	}
}

/**
 * One-line call to change the cursor to show the stopwatch symbol.
 */
void busy_cursor(void)
{
	SetCursor(&busy_curs);
}

/**
 * Centers the given window on the main graphics device.
 *
 * @param window  the window to center.
 */
void center_window(WindowPtr window)
{
	GrafPtr old_port;
	GDHandle gd;
	Rect r;
	short x, y;

	if (! window) return;

	gd = GetMainDevice();
	r = (*gd)->gdRect;
	x = (r.right - r.left) / 2;
	y = (r.bottom - r.top) / 2;

	GetPort(&old_port);
	SetPort(window);

	r = window->portRect;
	x = x - (r.right - r.left) / 2;
	y = y - (r.bottom - r.top) / 2;

	if (x < 20) x = 20;
	if (y < 20) y = 20;

	MovePortTo(x, y);

	SetPort(old_port);
}

/**
 * @return  true if WaitNextEvent is implemented
 */
Boolean init_is_wne(void)
{
	long v;

	/* check if on Mac Plus or later */
	if (Gestalt(gestaltMachineType, &v) && v >= 4) {
		/* see THINK Reference example in WaitNextEvent */
		return NGetTrapAddress(0xA860, 1) != NGetTrapAddress(0xA89F, 1);
	} else {
		return false;
	}
}

/**
 * Performs the usual Mac calls to start up a program and sets up the utility
 * functions for later use.
 *
 * @param *quit   pointer to a program quit function for fatal errors, 0 to use a
 *                default ExitToShell() call.
 * @param ptrcnt  the number of MoreMasters() invocations desired for the program.
 * @return        true if successful, false otherwise; this will not invoke the quit
 *                function on error but it would be wise for the caller to stop the
 *                program quickly, many things will likely not work on a failed call.
 */
Boolean init_program(void (*quit)(void), short ptrcnt)
{
	short i, err;
	CursHandle ch;

	InitGraf(&thePort);
	InitFonts();
	InitWindows();
	InitMenus();
	TEInit();
	InitDialogs(0L);
	InitCursor();

	quit_func = 0;

	MaxApplZone();
	for (i = 0; i < ptrcnt; i++) {
		MoreMasters();
		err = MemError();
		if (err) return false;
	}

	/* copy the stopwatch cursor locally for later use*/
	ch = GetCursor(watchCursor);
	if (ch) {
		HLock((Handle) ch);
		BlockMove(*ch, &busy_curs, sizeof(Cursor));
		HUnlock((Handle) ch);
		DisposHandle((Handle) ch);
	}

	/* now OK to assign per contract */
	quit_func = quit;
	return true;
}

/**
 * Generic memory error routine that should be called when the heap is exhausted
 * or a Memory Manager error occurs.
 *
 * This will post a basic message to the user and call the quit function (or just
 * ExitToShell()) to terminate the program immediately.
 */
void mem_fail(void)
{
	StopAlert(ALRT_UTIL_MEM_FAIL, 0);
	if (quit_func) {
		quit_func();
	}
	ExitToShell();
}

/**
 * Scans a Pascal string for instances of '^' and replaces with ' '. Useful for making
 * sure there are no inadvertent "^0" type components within a string.
 *
 * This could be a Munger() call if it were any more complicated (which it is not). It
 * would be interesting to check if that would be faster here though...
 *
 * @param s  the string to check.
 */
void repl_caret(unsigned char *s)
{
	short i, len;

	if (! s) return;

	len = s[0];
	for (i = 1; i <= len; i++) {
		if (s[i] == '^') s[i] = ' ';
	}
}

/**
 * Handles loading strings from resources when the final storage is limited by size.
 * This will pull the string from the resource and copy it into the storage, truncating
 * as needed.
 *
 * @param id    the STR# to load.
 * @param idx   the index within the string list.
 * @param *str  pointer to the resulting string storage.
 * @param size  the data size (_not_ string length) of the above storage.
 */
void str_load(short id, short idx, unsigned char *str, short size)
{
	short len;
	unsigned char* tmp;

	if (! str) return;
	if (size <= 0) return;

	if (! (tmp = (unsigned char *) NewPtr(256))) {
		mem_fail();
	}

	GetIndString(tmp, id, idx);
	if (tmp[0] > size - 1) {
		tmp[0] = size - 1;
	}

	if (tmp[0] > 0) {
		BlockMove(tmp, str, tmp[0] + 1);
	}

	DisposPtr((Ptr) tmp);
}
