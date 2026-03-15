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
#include "program.h"
#include "progress.h"
#include "window.h"
#include "util.h"

static unsigned short menu_state;

static void init_menus(void)
{
	MenuHandle h;
	short i;

	h = GetMenu(MENU_APPLE);
	AddResMenu(h, 'DRVR');
	InsertMenu(h, 0);

	for (i = MENU_FILE; i <= MENU_EDIT + PROGRAM_MENU_COUNT; i++) {
		h = GetMenu(i);
		InsertMenu(h, 0);
		if (i == 130 && PROGRAM_MENU_USE_EDIT == 0) {
			DisableItem(h, 0); /* gray out Edit to start with */
		}
	}

	DrawMenuBar();
}

static void update_menus(void)
{
	WindowPtr window;
	unsigned short next_menu_state;
	short kind;
	MenuHandle edit;

	/* figure out current state versus previous call */
	next_menu_state = program_state() & 0x7FFF;
	window = FrontWindow();
	if (window) {
		kind = ((WindowPeek) window)->windowKind;
		if (kind < userKind) {
			next_menu_state = next_menu_state | 0x8000;
		}
	} else {
		kind = userKind;
	}

	/* only update if there has been a change */
	if (next_menu_state != menu_state) {
		edit = GetMHandle(MENU_EDIT);

		if (! PROGRAM_MENU_USE_EDIT) {
			DisableItem(edit, 0);
		}

		if (kind < userKind) {
			/* DA can use Edit menu */
			EnableItem(edit, 0);
		}

		program_update_menus();

		menu_state = next_menu_state;
		DrawMenuBar();
	}
}

static void menu_command(long menu_key)
{
	short menu_id, menu_item, kind;
	unsigned char *item_ptr;
	WindowPtr window;

	menu_id = HiWord(menu_key);
	menu_item = LoWord(menu_key);

	/* check if DA */
	window = FrontWindow();
	if (window) {
		kind = ((WindowPeek) window)->windowKind;
	} else {
		kind = userKind;
	}

	switch (menu_id) {
	case MENU_APPLE:
		if (menu_item == 1) {
			program_menu_command(menu_id, menu_item);
		} else {
			if (! (item_ptr = (unsigned char *) NewPtr(256))) {
				mem_fail();
			}
			GetItem(GetMenuHandle(MENU_APPLE), menu_item, item_ptr);
			OpenDeskAcc(item_ptr);
			DisposPtr((Ptr) item_ptr);
		}
		break;
	case MENU_EDIT:
		if (kind < userKind) {
			/* delegate to DA */
			SystemEdit(menu_item);
		} else {
			program_menu_command(menu_id, menu_item);
		}
		break;
	default:
		program_menu_command(menu_id, menu_item);
	}

	HiliteMenu(0);
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
		if (progress_visible()) {
			if (ref == WIND_MAIN && active) {
				/* progress steals activation */
				progress_activate(true);
				window_activate(false);
			} else {
				progress_activate(active);
				window_activate(false);
			}
		} else if (window_visible()) {
			window_activate(active);
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
		menu_command(MenuSelect(evt->where));
		break;
	case inSysWindow:
		SystemClick(evt, window);
		break;
	case inContent:
		if (progress_visible()) {
			if (ref == WIND_PROGRESS) {
				if (window != FrontWindow()) {
					SelectWindow(window);
				} else {
					program_in_content_progress(evt);
				}
			} else if (ref == WIND_MAIN) {
				/* fake a modal dialog response */
				SysBeep(1);
			}
		} else if (window_visible()) {
			if (window != FrontWindow()) {
				SelectWindow(window);
			} else {
				program_in_content_window(evt);
			}
		}
		break;
	case inDrag:
		if (progress_visible() && ref == WIND_MAIN) {
			SysBeep(1);
		} else {
			DragWindow(window, evt->where, &(*GetGrayRgn())->rgnBBox);
		}
		break;
	case inGrow:
		if (progress_visible() && ref == WIND_MAIN) {
			SysBeep(1);
		} else {
			window_grow(evt->where);
		}
		break;
	case inZoomIn:
	case inZoomOut:
		if (TrackBox(window, evt->where, region)) {
			window_zoom(region);
		}
		break;
	case inGoAway:
		if (progress_visible() && ref == WIND_MAIN) {
			SysBeep(1);
		} else {
			if (TrackGoAway(window, evt->where)) {
				program_window_close();
			}
		}
		break;
	}
}

static void evt_keydown(EventRecord *evt, Boolean autokey)
{
	WindowPtr window;
	short kind;
	long ref;

	if (!autokey && (evt->modifiers & cmdKey)) {
		menu_command(MenuKey(evt->message & charCodeMask));
	} else {
		window = FrontWindow();
		if (!window) return;
		kind = ((WindowPeek) window)->windowKind;
		ref = ((WindowPeek) window)->refCon;

		if (kind == userKind && ref == WIND_MAIN) {
			program_window_key(evt, autokey);
		}
	}
}

static void evt_null(void)
{
	program_evt_null();
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

static void evt_os(EventRecord *evt)
{
	if (suspendResumeMessage & evt->message >> 24) {
		if (progress_visible()) {
			progress_resume(evt->message & resumeFlag);
		} else if (window_visible()) {
			window_resume(evt->message & resumeFlag);
		}
	}
}

int main(void)
{
	long i;
	EventRecord evt;

	if (! init_program(program_quit, 2)) {
		return 128;
	}

	config_init();

	/*
	 * THINK C has glue to make this call safe on <6.0.4.
	 * Not supposed to use gestaltSystemVersion like this but
	 * we will ignore that and note need for >=6.0.8, nothing
	 * lower is supported.
	 *
	 * Note: 6.0.8 reports as 0x0607
	 */
	if (! (trap_available(_Gestalt)
			&& !Gestalt(gestaltSystemVersion, &i)
			&& i >= 0x0607)) {
		CautionAlert(ALRT_BAD_VERSION, 0L);
	}

	init_menus();

	if(! (program_init() && window_init() && progress_init())) {
		mem_fail();
	}

	FlushEvents(everyEvent, 0);

	program_start();

	while (true) {

		if (g_use_wne) {
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

		update_menus();

		switch (evt.what) {
		case mouseDown:
			evt_mousedown(&evt);
			break;
		case keyDown:
			evt_keydown(&evt, false);
			break;
		case autoKey:
			evt_keydown(&evt, true);
			break;
		case updateEvt:
			evt_update(&evt);
			break;
		case activateEvt:
			evt_activate(&evt);
			break;
		case osEvt:
			evt_os(&evt);
			break;
		}
	}

	return 0;
}
