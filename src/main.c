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

static void evt_null(void)
{
	if (xfer_active) {
		busy_cursor();

		if (!transfer_tick()) {
			xfer_active = false;
			transfer_end();
			window_text(0);
			SysBeep(1);
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

	/* also disallow opening when a transfer is in progress */
	if (xfer_active) {
		DisableItem(file, 1);
	}
}

void do_open(void)
{
	Handle h;
	long err;
	short length;

	if (dialog_open(&scsi_id, &open_type)) {

		window_show(false);
		busy_cursor();

		if (err = scsi_list_files(scsi_id, open_type, &h, &length)) {
			alert_dual(ALRT_SCSI_ERROR, HiWord(err), LoWord(err));
		} else {
			window_populate(open_type, h, length);
			DisposHandle(h);
			SetCursor(&arrow);
			window_show(true);
		}
	}
}

void do_quit(void)
{
/* FIXME implement something more formal here */
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

void do_in_content(EventRecord *evt)
{
	short item;
	Str255 str;

	if (window_click(evt, &item)) {
		/* veto if a transfer is active */
		if (xfer_active) return;

		busy_cursor();

		if (open_type) {
			emu_mount(scsi_id, item);
			SetCursor(&arrow);
		} else {
			if (transfer_start(scsi_id, item)) {
				xfer_active = true;
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

	region = FindWindow(evt->where, &window);
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
		if (window != FrontWindow()) {
			SelectWindow(window);
		} else {
			do_in_content(evt);
		}
		break;
	case inDrag:
		DragWindow(window, evt->where, &(*GetGrayRgn())->rgnBBox);
		break;
	case inGrow:
		window_grow(evt->where);
		break;
	case inZoomIn:
	case inZoomOut:
		/* not implemented */
		break;
	case inGoAway:
		if (TrackGoAway(window, evt->where)) {
			window_show(false);
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

	if (!evt) return;
	window = (WindowPtr) evt->message;
	if (!window) return;
	kind = ((WindowPeek) window)->windowKind;

	if (kind == userKind) {
		window_update();
	}
}

void evt_activate(EventRecord *evt)
{
	short kind;
	WindowPtr window;

	if (!evt) return;
	window = (WindowPtr) evt->message;
	if (!window) return;
	kind = ((WindowPeek) window)->windowKind;

	if (kind == userKind) {
		window_activate(evt->modifiers & activeFlag);
	}
}

void evt_os(EventRecord *evt)
{
	if (suspendResumeMessage & evt->message >> 24) {
		window_resume(evt->message & resumeFlag);
	}
}

/* janky update text, will be replaced by proper xfer dialog at some point */
void evt_app3(EventRecord *evt)
{
	Str255 s;
	Str15 sn;
	unsigned char c, cn;
	long i;

	i = (long) evt->message;
	NumToString(i, sn);
	cn = (unsigned char) sn[0];
	sn[++cn] = '%';

	GetIndString(s, STR_GENERAL, STRI_GEN_DOWNLOAD);
	c = (unsigned char) s[0];

	BlockMove(&(sn[1]), &(s[c+1]), cn);
	s[0] = c + cn;

	window_text(s);
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
	if(!window_init()) {
		StopAlert(ALRT_MEM_ERROR, 0);
		ExitToShell();
	}

	xfer_active = false;
	scsi_id = 0;
	open_type = 0;

	FlushEvents(everyEvent, 0);
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
