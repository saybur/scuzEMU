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
#include "scsi.h"
#include "util.h"

short emu_count;
short emu_index[MAXIMUM_FILES];
long emu_sizes[MAXIMUM_FILES];

/**
 * Sets up the emulator processing / tracking logic. Mostly a placeholder.
 */
void emu_init(void)
{
	emu_count = 0;
}

/**
 * Provides information on the given item from the original query against the emulator.
 * This also does a range check and will return false if the item number is invalid.
 *
 * @param item   the item number in the original listing to fetch.
 * @param index  the file index identifier.
 * @param size   the file size in bytes.
 * @return       true if the item is within the range expected, false otherwise.
 */
Boolean emu_get_info(short item, short *index, long* size)
{
	if (item < 0 || item > emu_count) return false;

	*index = emu_index[item];
	*size = emu_sizes[item];
	return true;
}

/**
 * Parses a block of raw data from the SCSI device, inserting it into internal storage
 * /and/ the provided list for showing to the user.
 *
 * The format of this data is described in scsi.c. Input should be cleanly divisible by
 * 40; excess bytes will be discarded. This call will modify the data provided.
 *
 * This will clear any existing items.
 *
 * @param list      list to be updated.
 * @param data      data to be parsed, or NIL to clear list.
 * @param data_len  length of above data, or zero to clear list.
 * @return          the number of entries encountered.
 */
short emu_populate_list(ListHandle list, Handle data, short data_len)
{
	short dcnt, rcnt, i, j, t, min;
	Point p;
	unsigned char *d;
	short *offsets;

	/* resolve condition where no data is available */
	if (! (list && data && data_len > 40)) {
		emu_count = 0;
		if (list) {
			LDelRow(0, 0, list);
		}
		return 0;
	}

	HLock(data);
	d = (unsigned char *) (*data);

	/* delete all existing list rows */
	LDelRow(0, 0, list);
	emu_count = 0;

	/* figure out how many real entries there are, directories don't count */
	rcnt = 0;
	for (i = 0; i < data_len; i += 40) {
		/* directory = 0, files = 1 */
		if (d[i+1]) {
			rcnt++;

			/* while we are here ensure string is null terminated */
			d[i+34] = '\0';
		}
	}

	/* reserve space for tracking valid data offsets and list cells */
	if (! (offsets = (short *) NewPtr(rcnt * 2))) {
		HUnlock(data);
		StopAlert(ALRT_MEM_ERROR, 0);
		return;
	}
	LAddRow(rcnt, 0, list);

	/* find the data offset for each actual entry */
	t = 0;
	for (i = 0; i < data_len; i += 40) {
		if (! (d[i+1])) continue;
		offsets[t++] = i;
	}

	/* modify data to make names Pascal strings */
	for (i = 0; i < rcnt; i++) {
		t = offsets[i];
		for (j = 2; j <= 34; j++) {
			if (d[t+j] == '\0') {
				/* add Pascal length */
				d[t+1] = j - 2;
				break;
			}
		}
	}

	/* sort by file name */
	for (i = 0; i < rcnt - 1; i++) {
		min = i;
		for (j = i + 1; j < rcnt; j++) {
			t = RelString(
					&(d[offsets[j]+1]),
					&(d[offsets[min]+1]),
					false, false);
			if (t < 0) min = j;
		}
		if (min != i) {
			t = offsets[i];
			offsets[i] = offsets[min];
			offsets[min] = t;
		}
	}

	/* store data */
	for (i = 0; i < rcnt; i++) {
		t = offsets[i];

		/* insert filename into list */
		SetPt(&p, 0, i);
		LSetCell(&(d[t+2]), d[t+1], p, list);

		/* fetch remaining values */
		emu_index[emu_count] = d[t];
		emu_sizes[emu_count] = ((long) d[t+36] << 24)
				+ ((long) d[t+37] << 16)
				+ ((long) d[t+38] << 8)
				+ (long) d[t+39];
		emu_count++;
	}

	DisposPtr((Ptr) offsets);
	HUnlock(data);
}

/**
 * Mount the image at the given index.
 *
 * @param scsi_id  the target SCSI ID
 * @param item     the list cell row to mount.
 */
void emu_mount(short scsi_id, short item)
{
	short index;
	long err;

	if (item < emu_count) {
		index = emu_index[item];
		if (err = scsi_set_image(scsi_id, index)) {
			alert_dual(ALRT_SCSI_ERROR, HiWord(err), LoWord(err));
		} else {
			SetCursor(&arrow);
			NoteAlert(ALRT_IMAGE_CHANGE, 0);
		}
	} else {
		SetCursor(&arrow);
		alert_single(ALRT_GENERIC, 2);
	}
}
