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

/**
 * (Nominally) converts raw names from the SCSI device into Pascal strings for display.
 * However, the list seems to be happy with null-terminated strings. Leaving here in case
 * it's needed but at this point it's just an inefficient strlen.
 *
 * @param str  input string to convert.
 * @param max  maximum length to search in string.
 * @return     length of string or -1 if no null terminator found before max reached.
 */
static short emu_parse_string(char *str, short max)
{
	register short i;
	short len;

	len = -1;
	for (i = 0; i < max; i++) {
		if (str[i] == '\0') {
			len = i;
			break;
		}
	}

	/*
	 * if (len == -1) return -1;
	 * BlockMove(str, &str[1], len);
	 * str[0] = len;
	 */
	return len;
}

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
 * 40; excess bytes will be discarded.
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
	short lcnt, i, aidx, slen, rcnt;
	char str[33];
	Point p;
	unsigned char *arr;

	if (!list) {
/* probably should treat this as a more critical error */
/* TODO: for now just force-clear */
		data_len = 0;
	}

	/* resolve condition where no data available */
	lcnt = data_len / 40;
	if (!data || lcnt <= 0) {
		emu_count = 0;
		if (list) {
			LDelRow(0, 0, list);
		}
		return 0;
	}

	HLock(data);

	/* delete all existing rows */
	LDelRow(0, 0, list);
	emu_count = 0;

	/* figure out how many real entries there are, directories don't count */
	rcnt = 0;
	for (i = 0; i < data_len; i += 40) {
		/* directory = 0, files = 1 */
		if ((*data)[i + 1]) {
			rcnt++;
		}
	}
	LAddRow(rcnt, 0, list);

	for (i = 0; i < lcnt; i++) {
		arr = (unsigned char *) &((*data)[i * 40]);

		/* as above, skip directories */
		if (!arr[1]) {
			continue;
		}

		/* parse out the name of the file */
		BlockMove(&(arr[2]), str, 33);
		slen = emu_parse_string(str, 33);
		if (slen < 1) continue;

		/* insert filename into list */
		SetPt(&p, 0, i);
		LSetCell(str, slen, p, list);

		/* fetch remaining values */
		emu_index[emu_count] = arr[0];
		emu_sizes[emu_count] = ((long) arr[36] << 24)
				+ ((long) arr[37] << 16)
				+ ((long) arr[38] << 8)
				+ (long) arr[39];
		emu_count++;
	}

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
