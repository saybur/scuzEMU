/*
 * Copyright (C) 2024-2026 saybur
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
#include "dialog.h"
#include "emu.h"
#include "program.h"
#include "progress.h"
#include "scsi.h"
#include "window.h"
#include "transfer.h"
#include "upload.h"
#include "util.h"

/**
 * Called from main.c at the times described in the individual functions below.
 *
 * There are a number of assumptions made in the main code that the implementation
 * in this file needs to conform to:
 *
 * 1) There is a main window (window.c) that may be closeable,
 * 2) There is a quasi-modal window for progress reporting (progress.c)
 * 3) 'MENU' resources start at 128 and are defined as:
 *   *) 128: Apple
 *   *) 129: File
 *   *) 130: Edit (set PROGRAM_MENU_USE_EDIT to 1 if in use)
 *   *) 131+: any other menus up to PROGRAM_MENU_COUNT, in order
 * 4) Numerous other things I'm likely forgetting, update this as needed.
 *
 * Idea here is to decouple program logic from the byzantine Mac application boilerplate.
 * Suggestions from readers about other (smarter) ways to do this are quite welcome.
 */

#define STATE_IDLE      1
#define STATE_OPEN      2
#define STATE_DOWNLOAD  3
#define STATE_UPLOAD    4

static unsigned short pstate;
static short scsi_id;
static unsigned char tb_api;
static short open_type;
static Str255 work_dir;

/**
 * Performs a file/image list update from the emulator using stored values.
 * This will automatically show or hide the window based on the results of the
 * operation, updating the program state accordingly.
 */
static void do_list_update(void)
{
	Handle h;
	long err;
	short length, count;
	Str15 ns;

	busy_cursor();
	if (err = scsi_list_files(scsi_id, open_type, &h, &length)) {
		window_show(false);
		pstate = STATE_IDLE;
		scsi_alert(err);
	} else {
		if (length <= 0) {
			count = 0;
			/* handle never allocated, do not discard */
			window_populate(scsi_id, open_type, 0, 0);
		} else {
			count = window_populate(scsi_id, open_type, h, length);
			DisposHandle(h);
		}

		SetCursor(&arrow);
		if (open_type) {
			/* images */
			if (count <= 0) {
				/* probably not a valid device */
				window_show(false);
				NumToString(scsi_id, ns);
				ParamText(ns, 0, 0, 0);
				NoteAlert(ALRT_NO_IMAGES, 0);
				pstate = STATE_IDLE;
			} else {
				window_show(true);
				pstate = STATE_OPEN;
			}
		} else {
			/* files */
			if (count <= 0) {
				/* just mention issue, it might cause problems (or not) */
				NoteAlert(ALRT_NO_FILES, 0);
			}
			window_show(true);
			pstate = STATE_OPEN;
		}
	}
}

static void do_xfer_stop()
{
#if 0
	Str15 num;

	if (pstate == STATE_DOWNLOAD) {
		NumToString(transfer_time(), num);
		ParamText(num, 0, 0, 0);
		NoteAlert(ALRT_GENERIC, 0);
	}
#endif

	if (pstate == STATE_DOWNLOAD) {
		pstate = STATE_OPEN;
		transfer_end();
		progress_show(false);
		window_text(0);
	} else if (pstate == STATE_UPLOAD) {
		upload_end();
		progress_show(false);
		window_text(0);
		do_list_update();
	}
}

static void do_open(void)
{
	Boolean is_emu;
	long err;
	short s, o;

	s = scsi_id;
	o = open_type;

	if (dialog_open(&s, &o)
			&& config_check_mode(s)) {
		scsi_id = s;
		open_type = o;
		do_list_update();
	}
}

static void do_upload(void)
{
	if (pstate == STATE_OPEN && !open_type && upload_start(scsi_id)) {
		pstate = STATE_UPLOAD;
		progress_set_direction(false);
		progress_show(true);
	}
}

static void do_directory(void)
{
	unsigned char *work_dir;
	long err;

	if (pstate == STATE_OPEN && !open_type) {
		/* double check if device can do this */
		if (! config_has_capability(scsi_id, CAP_WORK_DIRECTORY)) {
			return;
		}

		/* fetch current directory state */
		if (! (work_dir = (unsigned char *) NewPtr(256))) {
			mem_fail();
		}
		if (err = scsi_get_work_directory(scsi_id, work_dir)) {
			scsi_alert(err);
			DisposPtr((char *) work_dir);
			return;
		}

		/* prompt user to change it */
		if (dialog_change_working_directory(work_dir)) {
			/* update device with legal value & redraw with changes */
			if (work_dir[0] > 63) work_dir[0] = 63;
			if (err = scsi_set_work_directory(scsi_id, work_dir)) {
				scsi_alert(err);
			}
			do_list_update();
		}

		DisposPtr((char *) work_dir);
	}
}

static void do_download(void)
{
	Str15 str;

	if (pstate != STATE_OPEN) return;

	busy_cursor();

	if (open_type) {
		emu_mount(scsi_id);
		SetCursor(&arrow);
	} else {
		if (transfer_start(scsi_id)) {
			pstate = STATE_DOWNLOAD;
			str_load(STR_GENERAL, STRI_GEN_DOWNLOAD, str, 16);
			window_text(str);

			progress_set_file(str);
			progress_set_direction(true);
			progress_show(true);
		} else {
			/* failed to start the transfer */
			SetCursor(&arrow);
		}
	}
}

/**
 * Called when the program has received a null event, either foreground or background.
 */
void program_evt_null(void)
{
	if (pstate == STATE_DOWNLOAD) {
		if (! transfer_tick()) {
			do_xfer_stop();
		}
	} else if (pstate == STATE_UPLOAD) {
		if (! upload_tick()) {
			do_xfer_stop();
		}
	} else {
		SetCursor(&arrow);
	}
}

/**
 * Called when the program detects an event within the window.
 */
void program_in_content_progress(EventRecord *evt)
{
	if (progress_click(evt)) {
		/* user clicked stop button */
		do_xfer_stop();
	}
}

/**
 * Called when the program detects an event within the window.
 */
void program_in_content_window(EventRecord *evt)
{
	Boolean xfer;

	window_click(evt, &xfer);

	if (xfer) {
		do_download();
	}
}

/**
 * Called during program startup. This is required to set program_state() to a nonzero
 * value but is otherwise free to do anything else convenient for the program.
 */
Boolean program_init(void)
{
	emu_init();
	pstate = STATE_IDLE;
}

/**
 * Called when a menu command is received from either a click or a key shortcut. The
 * main program will handle Apple menu items (other than "About...") and Edit
 * activations for DAs.
 */
void program_menu_command(short menu_id, short menu_item)
{
	switch (menu_id) {
	case MENU_APPLE:
		/* menu_item is always 1 here */
		Alert(ALRT_ABOUT, 0L);
		break;
	case MENU_FILE:
		if (menu_item == MENUI_OPEN) {
			do_open();
		} else if (menu_item == MENUI_UPLOAD) {
			do_upload();
		} else if (menu_item == MENUI_DIRECTORY) {
			do_directory();
		} else if (menu_item == MENUI_QUIT) {
			program_quit();
		}
		break;
	}
}

/**
 * Quits the program. This *must* be safe to call at all times, even before program_init()
 * is invoked.
 */
void program_quit(void)
{
	do_xfer_stop();
	ExitToShell();
}

/**
 * Called after program startup to allow the program to do something prior to the main
 * event loop beginning. This is free to do nothing.
 */
void program_start(void)
{
	do_open();
}

/**
 * Provides a value indicating a general state of the program. This is not used for much
 * other than to help trigger a redraw of menus. It is required to return 0 prior to
 * program_init() and nonzero afterwards.
 *
 * @return a value describing the state of the program, as above.
 */
unsigned short program_state(void)
{
	return pstate;
}

/**
 * Called in response to the user trying to close the main window.
 */
void program_window_close(void)
{
	window_show(false);
	pstate = STATE_IDLE;
}

/**
 * Called in response to a key event on the main window.
 */
void program_window_key(EventRecord *evt, Boolean autokey)
{
	char k;

	k = evt->message & charCodeMask;
	if (!autokey && (k == 0x03 || k == 0x0D)) {
		/* enter/return key */
		do_download();
	} else {
		window_key(evt);
	}
}

/**
 * Called in response to a change in program_state() to update menus.
 */
void program_update_menus(void)
{
	MenuHandle file;

	file = GetMHandle(129);

	/* set default File state */
	EnableItem(file, MENUI_OPEN);
	DisableItem(file, MENUI_UPLOAD);
	DisableItem(file, MENUI_DIRECTORY);
	EnableItem(file, MENUI_QUIT);

	/* disallow opening while a transfer is in progress */
	if (pstate == STATE_DOWNLOAD || pstate == STATE_UPLOAD) {
		DisableItem(file, MENUI_OPEN);
	}

	/* allow uploading only when we are connected & have files */
	if (pstate == STATE_OPEN && !open_type) {
		EnableItem(file, MENUI_UPLOAD);
		/* and allow changing the working directory only if possible */
		if (config_has_capability(scsi_id, CAP_WORK_DIRECTORY)) {
			EnableItem(file, MENUI_DIRECTORY);
		}
	}
}
