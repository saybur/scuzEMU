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
#include "dialog.h"
#include "emu.h"
#include "progress.h"
#include "scsi.h"
#include "window.h"
#include "transfer.h"
#include "upload.h"
#include "util.h"

#define STATE_IDLE      1
#define STATE_OPEN      2
#define STATE_DOWNLOAD  3
#define STATE_UPLOAD    4

static short scsi_id;
static short open_type;
static short pstate;

static void init_menus(void)
{
	MenuHandle h;

	h = GetMenu(MENU_APPLE);
	AddResMenu(h, 'DRVR');
	InsertMenu(h, 0);

	h = GetMenu(MENU_FILE);
	InsertMenu(h, 0);

	h = GetMenu(MENU_EDIT);
	InsertMenu(h, 0);
	DisableItem(h, 0); /* gray out to start with */

	DrawMenuBar();
}

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
		alert_template_error(0, ALRT_SCSI_ERROR, HiWord(err), LoWord(err));
	} else {
		if (length <= 0) {
			count = 0;
			/* handle never allocated, do not discard */
		} else {
			count = window_populate(open_type, h, length);
			DisposHandle(h);
		}

		SetCursor(&arrow);
		if (open_type) {
			/* images */
			if (count <= 0) {
				/* probably not a valid device */
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

static void evt_null(void)
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

static void update_menus(EventRecord *evt)
{
	WindowPtr window;
	short kind;
	MenuHandle file, edit;

	if (!evt) return;

	file = GetMHandle(MENU_FILE);
	edit = GetMHandle(MENU_EDIT);

	/* by default allow "Open..." */
	EnableItem(file, 1);
	/* and disallow Edit, we don't use it */
	DisableItem(edit, 0);

	window = (WindowPtr) evt->message;
	if (window) {
		kind = ((WindowPeek) window)->windowKind;

		if (kind != userKind) {
			/* enable edit menu for DAs */
			EnableItem(edit, 0);
			/* disallow opening when the DA is active */
			DisableItem(file, 1);
		}
	}

	/* allow uploading only when we are connected to a device with a file list */
	if (pstate == STATE_OPEN && !open_type) {
		EnableItem(file, MENUI_UPLOAD);
	} else {
		DisableItem(file, MENUI_UPLOAD);
	}

	/* also disallow opening while a transfer is in progress */
	if (pstate == STATE_DOWNLOAD || pstate == STATE_UPLOAD) {
		DisableItem(file, MENUI_OPEN);
	}
}

static void do_open(void)
{
	if (dialog_open(&scsi_id, &open_type)) {
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

static void do_quit(void)
{
	do_xfer_stop();
	ExitToShell();
}

static void do_menu_command(long menu_key)
{
	short menu_id, menu_item;
	unsigned char *item_ptr;

	menu_id = HiWord(menu_key);
	menu_item = LoWord(menu_key);

	switch (menu_id) {
	case MENU_APPLE:
		if (menu_item == 1) {
			Alert(ALRT_ABOUT, 0L);
		} else {
			if (! (item_ptr = (unsigned char *) NewPtr(256))) {
				mem_fail();
			}
			GetItem(GetMenuHandle(MENU_APPLE), menu_item, item_ptr);
			OpenDeskAcc(item_ptr);
			DisposPtr((Ptr) item_ptr);
		}
		break;
	case MENU_FILE:
		if (menu_item == MENUI_OPEN) {
			do_open();
		} else if (menu_item == MENUI_UPLOAD) {
			do_upload();
		} else if (menu_item == MENUI_QUIT) {
			do_quit();
		}
		break;
	case MENU_EDIT:
		/* delegate to DA, we don't use these */
		SystemEdit(menu_item);
		break;
	}

	HiliteMenu(0);
}

static void do_in_content_progress(EventRecord *evt)
{
	if (progress_click(evt)) {
		/* user clicked stop button */
		do_xfer_stop();
	}
}

static void do_in_content_window(EventRecord *evt)
{
	short itemcnt;
	Str15 str;

	window_click(evt, &itemcnt);

	if (itemcnt > 0) {
		/* veto if a transfer is active */
		if (pstate != STATE_OPEN) return;

		busy_cursor();

		if (open_type) {
			emu_mount(scsi_id);
			SetCursor(&arrow);
		} else {
			if (transfer_start(scsi_id, &itemcnt)) {
				pstate = STATE_DOWNLOAD;
				str_load(STR_GENERAL, STRI_GEN_DOWNLOAD, str, 16);
				window_text(str);

				progress_set_percent(0);
				progress_set_count(itemcnt);
				progress_set_file(str);
				progress_set_direction(true);
				progress_show(true);
			} else {
				/* failed to start the transfer */
				SetCursor(&arrow);
			}
		}
	}
}

static void evt_mousedown(EventRecord *evt)
{
	short region;
	WindowPtr window;
	long ref;

	region = FindWindow(evt->where, &window);
	if (window) {
		ref = ((WindowPeek) window)->refCon;
	} else {
		ref = 0;
	}

	switch (region) {
	case inDesk:
		/* ignore condition */
		break;
	case inMenuBar:
		do_menu_command(MenuSelect(evt->where));
		break;
	case inSysWindow:
		SystemClick(evt, window);
		break;
	case inContent:
		if (pstate != STATE_OPEN) {
			if (ref == WIND_PROGRESS) {
				if (window != FrontWindow()) {
					SelectWindow(window);
				} else {
					do_in_content_progress(evt);
				}
			} else if (ref == WIND_MAIN) {
				/* fake a modal dialog response */
				SysBeep(1);
			}
		} else {
			if (window != FrontWindow()) {
				SelectWindow(window);
			} else {
				do_in_content_window(evt);
			}
		}
		break;
	case inDrag:
		if (pstate != STATE_OPEN && ref == WIND_MAIN) {
			SysBeep(1);
		} else {
			DragWindow(window, evt->where, &(*GetGrayRgn())->rgnBBox);
		}
		break;
	case inGrow:
		if (pstate != STATE_OPEN && ref == WIND_MAIN) {
			SysBeep(1);
		} else {
			window_grow(evt->where);
		}
		break;
	case inZoomIn:
	case inZoomOut:
		/* not implemented */
		break;
	case inGoAway:
		if (pstate != STATE_OPEN && ref == WIND_MAIN) {
			SysBeep(1);
		} else {
			if (TrackGoAway(window, evt->where)) {
				window_show(false);
				pstate = STATE_IDLE;
			}
		}
		break;
	}
}

static void evt_keydown(EventRecord *evt)
{
	char k;

	k = evt->message & charCodeMask;
	if (evt->modifiers & cmdKey) {
		do_menu_command(MenuKey(k));
	}
}

static void evt_update(EventRecord *evt)
{
	short kind;
	WindowPtr window;
	long ref;

	if (!evt) return;
	window = (WindowPtr) evt->message;
	if (!window) return;
	kind = ((WindowPeek) window)->windowKind;
	ref = ((WindowPeek) window)->refCon;

	if (kind == userKind) {
		if (ref == WIND_MAIN) {
			window_update();
		} else if (ref == WIND_PROGRESS) {
			progress_update();
		}
	}
}

static void evt_activate(EventRecord *evt)
{
	short kind;
	WindowPtr window;
	long ref;
	Boolean active;

	if (!evt) return;
	window = (WindowPtr) evt->message;
	if (!window) return;
	kind = ((WindowPeek) window)->windowKind;
	ref = ((WindowPeek) window)->refCon;
	active = evt->modifiers & activeFlag;

	if (kind == userKind) {
		if (pstate != STATE_OPEN) {
			if (ref == WIND_MAIN && active) {
				/* progress steals activation */
				progress_activate(true);
				window_activate(false);
			} else {
				progress_activate(active);
				window_activate(false);
			}
		} else {
			window_activate(active);
		}
	}
}

static void evt_os(EventRecord *evt)
{
	if (suspendResumeMessage & evt->message >> 24) {
		if (pstate != STATE_OPEN) {
			progress_resume(evt->message & resumeFlag);
		} else {
			window_resume(evt->message & resumeFlag);
		}
	}
}

int main(void)
{
	long i;
	EventRecord evt;
	Boolean wneAvail;

	if (! init_program(do_quit, 2)) {
		return 128;
	}

	/*
	 * Set early to avoid an unsafe do_quit()
	 */
	pstate = STATE_IDLE;

	/*
	 * THINK C has glue to make this call safe on <6.0.4.
	 * Not supposed to use gestaltSystemVersion like this but
	 * we will ignore that and note need for >=6.0.8, nothing
	 * lower is supported.
	 *
	 * Note: 6.0.8 reports as 0x0607
	 */
	if (!Gestalt(gestaltSystemVersion, &i) && i < 0x0607) {
		CautionAlert(ALRT_BAD_VERSION, 0L);
	}

	config_init();
	init_menus();
	wneAvail = init_is_wne();

	emu_init();
	if(! (window_init() && progress_init())) {
		mem_fail();
	}

	scsi_id = 0;
	open_type = 0;

	FlushEvents(everyEvent, 0);

	/* prompt open on app start */
	do_open();

	while (true) {

		if (wneAvail) {
			if (!WaitNextEvent(everyEvent, &evt, WAIT_EVENT_SLEEP, 0L)) {
				evt_null();
				continue;
			}
		} else {
			SystemTask();
			if (!GetNextEvent(everyEvent, &evt)) {
				evt_null();
				continue;
			}
		}

		switch (evt.what) {
		case mouseDown:
			evt_mousedown(&evt);
			break;
		case keyDown:
			evt_keydown(&evt);
			break;
		case updateEvt:
			update_menus(&evt);
			evt_update(&evt);
			break;
		case activateEvt:
			update_menus(&evt);
			evt_activate(&evt);
			break;
		case osEvt:
			evt_os(&evt);
			break;
		}
	}

	return 0;
}
