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

#ifndef __UTIL__
#define __UTIL__

void alert_single(short res_id, short str_id);
void alert_dual(short res_id, short str_id, short code);
void arr_del_short(short *arr, short len, short itm);
void busy_cursor(void);
void center_window(WindowPtr window);
void str_load(short list_id, short index, unsigned char *tmp, unsigned char *str, short mlen);

void util_init(void);

#endif /* __UTIL__ */
