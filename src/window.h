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

#ifndef __WINDOWH__
#define __WINDOWH__

Boolean window_init(void);

void window_click(EventRecord *evt, Boolean *dclick);
void window_key(EventRecord *evt);
void window_next(short *i);
void window_get_item_name(short item, Str255 str);
short window_populate(short scsi, short mode, Handle h, short count);

void window_activate(Boolean active);
void window_grow(Point p);
void window_resume(Boolean resume);
void window_show(Boolean show);
void window_text(unsigned char *str);
void window_update(void);

#endif /* __WINDOWH__ */
