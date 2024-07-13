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

#ifndef __SCSIH__
#define __SCSIH__

void scsi_alert(long fail);

long scsi_get_emu_api(short scsi_id, Boolean *valid, unsigned char *ver);
long scsi_list_files(short scsi_id, short open_type, Handle *data, short *length);
long scsi_read_file(short scsi_id, short index, long offset, char *data, short length);
long scsi_set_image(short scsi_id, short index);
long scsi_write_start(short scsi_id, unsigned char* name);
long scsi_write_block(short scsi_id, long offset, char *data, short length);
long scsi_write_end(short scsi_id);

#endif /* __SCSIH__ */
