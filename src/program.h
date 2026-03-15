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

#ifndef __PROGRAMH__
#define __PROGRAMH__

#define PROGRAM_MENU_COUNT     0
#define PROGRAM_MENU_USE_EDIT  0

void program_evt_null(void);
void program_in_content_progress(EventRecord *evt);
void program_in_content_window(EventRecord *evt);
Boolean program_init(void);
void program_menu_command(short menu_id, short menu_item);
void program_quit(void);
void program_start(void);
unsigned short program_state(void);
void program_window_close(void);
void program_window_key(EventRecord *evt, Boolean autokey);
void program_update_menus(void);

#endif /* __PROGRAMH__ */
