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
#include "util.h"

#define FNAME_MAX_LEN 31

static WindowPtr window;
static ControlHandle stop_button;
static Rect progress_bar;
static Str31 str_copying;
static Str15 str_writing;

static unsigned char fname[FNAME_MAX_LEN + 1];
static short fcount, progress;

/**
 * Draws just the progress bar. Should be called with the GrafPort set.
 *
 * @param *bounds  window->portRect
 * @param redraw   erase before drawing if true
 */
static void progress_draw_bar(Rect *bounds, Boolean redraw)
{
	Rect bar;

	bar.top = progress_bar.top + bounds->top;
	bar.bottom = progress_bar.bottom + bounds->top;
	bar.left = progress_bar.left + bounds->left;
	bar.right = progress_bar.right + bounds->left;

	if (redraw) {
		EraseRect(&bar);
		FrameRect(&bar);
	}
	bar.right = bar.left + 2 * progress;
	FillRect(&bar, &(qd.black));
}

/**
 * Draws the progress window.
 */
static void progress_draw(void)
{
	GrafPtr old_port;
	Rect bounds;
	short i, old_font;
	Str15 fcount_str;

	GetPort(&old_port);
	SetPort(window);

	EraseRect(&(window->portRect));

	UpdateControls(window, window->visRgn);

	bounds = window->portRect;

	old_font = thePort->txFont;
	TextFont(0);
	TextSize(12);

	/* "Remaining files to copy:" */
	MoveTo(bounds.left + 10, bounds.top + 20);
	DrawString(str_copying);

	/* "[num files to copy]" */
	NumToString((long) fcount, fcount_str);
	MoveTo(bounds.right - StringWidth(fcount_str) - 20, bounds.top + 20);
	DrawString(fcount_str);

	/* "Writing:" */
	MoveTo(bounds.left + 10, bounds.top + 42);
	DrawString(str_writing);

	/* "[filename]" */
	MoveTo(bounds.left + 70, bounds.top + 42);
	DrawString(fname);

	TextFont(old_font);
	TextSize(0);

	progress_draw_bar(&bounds, true);

	SetPort(old_port);
}

/**
 * Handles /activateEvt/.
 *
 * @param active  true if activating, false if inactivating.
 */
void progress_activate(Boolean active)
{
	GrafPtr old_port;

	GetPort(&old_port);
	SetPort(window);

	HLock((Handle) stop_button);
	HiliteControl(stop_button, (active ? 0 : 255));
	HUnlock((Handle) stop_button);

	SetPort(old_port);
}

/**
 * Handles /inContent/ on /mouseDown/.
 *
 * @param evt  the event record causing the click.
 * @return     true if the transfer was cancelled, false otherwise.
 */
Boolean progress_click(EventRecord *evt)
{
	short ctlp, r;
	ControlHandle ch;
	GrafPtr old_port;

	GetPort(&old_port);
	SetPort(window);

	GlobalToLocal(&(evt->where));

	r = 0;
	ctlp = FindControl(evt->where, window, &ch);
	if (ctlp == inButton) {
		HLock((Handle) stop_button);
		r = TrackControl(stop_button, evt->where, 0);
		HUnlock((Handle) stop_button);
	}

	SetPort(old_port);

	return r;
}

/**
 * Called during program startup to build the progress 'dialog' window. As with
 * window.c, only one instance of the progress monitor is needed.
 *
 * The window will still be hidden after this call.
 *
 * @return  true if success, false if failed due to lack of memory.
 */
Boolean progress_init(void)
{
	Str255 tmp;
	Rect bounds;

	window = GetNewWindow(WIND_PROGRESS, 0, (WindowPtr)-1);
	if (!window) {
		return false;
	}
	SetWRefCon(window, WIND_PROGRESS);

	str_load(STR_GENERAL, STRI_GEN_COPYING, tmp, str_copying, 32);
	str_load(STR_GENERAL, STRI_GEN_WRITING, tmp, str_writing, 16);

	progress = 0;
	fcount = 0;
	fname[0] = 0;

	SetPort(window);
	bounds = window->portRect;
	center_window(window);

	SetRect(&progress_bar, bounds.left + 10,
			bounds.top + 56,
			bounds.left + 210,
			bounds.top + 66);

	stop_button = GetNewControl(CNTL_STOP, window);
	if (! stop_button) {
		return false;
	}

	return true;
}

/**
 * Handles /osEvt/.
 *
 * @param resume true if resuming, false if suspending.
 */
void progress_resume(Boolean resume)
{
	if (resume) {
		SelectWindow(window);
	}
	HiliteWindow(window, resume);
	progress_activate(resume);
}

/**
 * Sets the number of remaining files in the operation.
 *
 * This triggers redrawing; call this _last_ when making an update.
 *
 * @param percent  the number of remaining files.
 */
void progress_set_count(short count)
{
	fcount = count;
	progress_draw();
}

/**
 * Sets the amount of progress as an integer percentage.
 *
 * @param percent  the value for progress, between 0 and 100 inclusive.
 */
void progress_set_percent(short percent)
{
	GrafPtr old_port;
	Rect bar;

	GetPort(&old_port);
	SetPort(window);

	if (percent < 0) percent = 0;
	if (percent > 100) percent = 100;
	progress = percent;

	progress_draw_bar(&(window->portRect), false);

	SetPort(window);
}

/**
 * Sets the active filename being processed.
 *
 * @param str  the Pascal style string to set.
 */
void progress_set_file(StringPtr str)
{
	unsigned char len;

	if (!str) {
		fname[0] = 0;
		return;
	}

	len = str[0];
	if (len > FNAME_MAX_LEN) {
		fname[FNAME_MAX_LEN - 2] = '.';
		fname[FNAME_MAX_LEN - 1] = '.';
		fname[FNAME_MAX_LEN] = '.';
		str[0] = FNAME_MAX_LEN;
		BlockMove(&(str[1]), &(fname[1]), FNAME_MAX_LEN - 3);
	} else {
		BlockMove(str, fname, len + 1);
	}
}

/**
 * Shows or hides the progress window, without doing disposal.
 *
 * @param open true to show, false to hide.
 */
void progress_show(Boolean open)
{
	if (open) {
		ShowWindow(window);
		SelectWindow(window);
	} else {
		HideWindow(window);
	}
}

/**
 * Handles /updateEvt/.
 */
void progress_update(void)
{
	BeginUpdate(window);
	progress_draw();
	EndUpdate(window);
}