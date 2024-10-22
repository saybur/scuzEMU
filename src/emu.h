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

#ifndef __EMUH__
#define __EMUH__

void emu_init(void);
short emu_get_count(void);
Boolean emu_get_info(short item, short *index, long* size);
short emu_populate_list(ListHandle list, Handle data, short length);
void emu_mount(short scsi_id);

#endif /* __EMUH__ */
