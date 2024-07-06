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
#include "dialog.h"
#include "emu.h"
#include "progress.h"
#include "scsi.h"
#include "window.h"
#include "transfer.h"
#include "util.h"

static void init_macintosh(void)
{
	long i;

	InitGraf(&thePort);
	InitFonts();
	InitWindows();
	InitMenus();
	TEInit();
	InitDialogs(0L);
	InitCursor();

	MaxApplZone();
	MoreMasters();
	MoreMasters();

	/*
	 * THINK C has glue to make this call safe on <6.0.4.
	 * Not supposed to use gestaltSystemVersion like this but
	 * we will ignore that and note need for >=6.0.8, nothing
	 * lower is supported.
	 *
	 * Note: 6.0.8 reports as 0x0607
	 */
	if (Gestalt(gestaltSystemVersion, &i)) {
		goto bad_version;
	}
	if (i < 0x0607) {
		goto bad_version;
	}

	return;

bad_version:
	CautionAlert(ALRT_BAD_VERSION, 0L);
	/* still allow user to run */
}

/* checks if WaitNextEvent is implemented */
static Boolean init_is_wne(void)
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

short scsi_id;
short open_type;
Boolean xfer_active;

static void do_xfer_stop()
{
	if (xfer_active) {
		xfer_active = false;
		transfer_end();
		progress_show(false);
		window_text(0);
	}
}

static void evt_null(void)
{
	if (xfer_active) {
		busy_cursor();
		PostEvent(app3Evt, 0);
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

	/* also disallow opening when a transfer is in progress */
	if (xfer_active) {
		DisableItem(file, 1);
	}
}

void do_open(void)
{
	Handle h;
	long err;
	short length, count;
	Str15 ns;

	if (dialog_open(&scsi_id, &open_type)) {

		window_show(false);
		busy_cursor();

		if (err = scsi_list_files(scsi_id, open_type, &h, &length)) {
			alert_dual(ALRT_SCSI_ERROR, HiWord(err), LoWord(err));
		} else {
			if (length <= 0) {
				count = 0;
				/* handle never allocated, do not discard */
			} else {
				count = window_populate(open_type, h, length);
				DisposHandle(h);
			}

			SetCursor(&arrow);
			if (count > 0) {
				window_show(true);
			} else {
				if (open_type) {
					NumToString(scsi_id, ns);
					ParamText(ns, 0, 0, 0);
					NoteAlert(ALRT_NO_IMAGES, 0);
				} else {
					/* no need to report ID, shared dir is global */
					NoteAlert(ALRT_NO_FILES, 0);
				}
			}
		}
	}
}

void do_quit(void)
{
	do_xfer_stop();
	ExitToShell();
}

void do_menu_command(long menu_key)
{
	short menu_id, menu_item;
	Str255 item_name;

	menu_id = HiWord(menu_key);
	menu_item = LoWord(menu_key);

	switch (menu_id) {
	case MENU_APPLE:
		if (menu_item == 1) {
			Alert(ALRT_ABOUT, 0L);
		} else {
			GetItem(GetMenuHandle(MENU_APPLE), menu_item, item_name);
			OpenDeskAcc(item_name);
		}
		break;
	case MENU_FILE:
		if (menu_item == 1) {
			do_open();
		} else if (menu_item == 3) {
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

void do_in_content_progress(EventRecord *evt)
{
	if (progress_click(evt)) {
		/* user clicked stop button */
		do_xfer_stop();
	}
}

void do_in_content_window(EventRecord *evt)
{
	short items[256];
	short itemcnt;
	Str255 str;

	window_click(evt, items, &itemcnt);

	if (itemcnt > 0) {
		/* veto if a transfer is active */
		if (xfer_active) return;

		busy_cursor();

		if (open_type) {
			emu_mount(scsi_id, items[0]);
			SetCursor(&arrow);
		} else {
			if (transfer_start(scsi_id, items, &itemcnt)) {
				xfer_active = true;
				GetIndString(str, STR_GENERAL, STRI_GEN_DOWNLOAD);
				window_text(str);

				progress_set_percent(0);
				progress_set_count(itemcnt);
				progress_set_file(str);
				progress_show(true);
			} else {
				/* failed to start the transfer */
				SetCursor(&arrow);
			}
		}
	}
}

void evt_mousedown(EventRecord *evt)
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
		if (xfer_active) {
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
		if (xfer_active && ref == WIND_MAIN) {
			SysBeep(1);
		} else {
			DragWindow(window, evt->where, &(*GetGrayRgn())->rgnBBox);
		}
		break;
	case inGrow:
		if (xfer_active && ref == WIND_MAIN) {
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
		if (xfer_active && ref == WIND_MAIN) {
			SysBeep(1);
		} else {
			if (TrackGoAway(window, evt->where)) {
				window_show(false);
			}
		}
		break;
	}
}

void evt_keydown(EventRecord *evt)
{
	char k;

	k = evt->message & charCodeMask;
	if (evt->modifiers & cmdKey) {
		do_menu_command(MenuKey(k));
	}
}

void evt_update(EventRecord *evt)
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

void evt_activate(EventRecord *evt)
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
		if (xfer_active) {
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

void evt_os(EventRecord *evt)
{
	if (suspendResumeMessage & evt->message >> 24) {
		if (xfer_active) {
			progress_resume(evt->message & resumeFlag);
		} else {
			window_resume(evt->message & resumeFlag);
		}
	}
}

void evt_app3(EventRecord *evt)
{
	if (!transfer_tick()) {
		do_xfer_stop();
	}
}

int main(void)
{
	EventRecord evt;
	Boolean wneAvail;

	init_macintosh();
	init_menus();
	wneAvail = init_is_wne();

	emu_init();
	util_init();
	if(! (window_init() && progress_init())) {
		StopAlert(ALRT_MEM_ERROR, 0);
		ExitToShell();
	}

	xfer_active = false;
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

		update_menus(&evt);

		switch (evt.what) {
		case mouseDown:
			evt_mousedown(&evt);
			break;
		case keyDown:
			evt_keydown(&evt);
			break;
		case updateEvt:
			evt_update(&evt);
			break;
		case activateEvt:
			evt_activate(&evt);
			break;
		case app3Evt:
			evt_app3(&evt);
			break;
		case osEvt:
			evt_os(&evt);
			break;
		}
	}
}
