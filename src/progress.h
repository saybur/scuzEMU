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

#ifndef __PROGRESS__
#define __PROGRESS__

void progress_activate(Boolean active);
Boolean progress_click(EventRecord *evt);
Boolean progress_init(void);
void progress_resume(Boolean resume);
void progress_show(Boolean open);
void progress_update(void);

void progress_set_count(short count);
void progress_set_file(StringPtr str);
void progress_set_percent(short percent);

#endif /* __PROGRESS__ */
