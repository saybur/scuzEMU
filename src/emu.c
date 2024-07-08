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

static short emu_count;
static short emu_index[MAXIMUM_FILES];
static long emu_sizes[MAXIMUM_FILES];

/**
 * Perform an ejection of the device at the given SCSI ID.
 *
 * Getting from SCSI ID down to the mounted volume has some steps, and I'm
 * not 100% sure the below code captures all possibilities. That said, here's
 * a rough overview of the process involved:
 *
 * 1) Get IO ref num, which is -(32+SCSI_ID)-1 (noted in DV18).
 * 2) Check if a matching device is present; if not, it's probably a sign that
 *    no driver is loaded, so there's no point in trying to eject.
 * 3) If a drive does exist, try to find the matching volume. This may not exist
 *    either, but that could be because there isn't a HFS volume to mount on it.
 * 4) Assuming a volume exists, unmount it.
 * 5) Regardless if a volume exists, call into the driver and eject the disk.
 * 6) Wait for a bit to allow the OS to update everything.
 *
 * This will pop up messages to the user that try to let them know if something
 * goes awry.
 *
 * Trying to eject at the volume level doesn't seem to execute the required
 * re-insertion code on the emulator I'm using to test, you do need to call into
 * the driver for that to happen. Apparently.
 *
 * Input on any of this would be appreciated, I feel like this is an area where
 * I'm missing something obvious within the OS.
 *
 * @param scsi_id  the SCSI ID to try to eject.
 * @return         true if ejection succeeded, false otherwise.
 */
static Boolean emu_eject(short scsi_id)
{
	QHdrPtr qhp;
	DrvQEl *qep;
	short iref, dnum, vref, err;
	long free;
	CntrlParam *ctrlp;
	unsigned char *diskstate;
	unsigned char *fname;

	/* see if a physical drive matches the ID */
	iref = -33 - scsi_id;
	dnum = 0;
	qhp = GetDrvQHdr();
	qep = (DrvQEl *) qhp->qHead;
	do {
		if (qep->dQRefNum == iref) {
			dnum = qep->dQDrive;
			break;
		}

		qep = (DrvQEl *) qep->qLink;
	} while (qep);
	if (! dnum) {
		alert_template(0, ALRT_GENERIC, STRI_GA_NO_DEV);
		return false;
	}

	/* store information about whether a disk is present */
	diskstate = (unsigned char *) qep;
	diskstate -= 3;

	/* required for next call, otherwise not useful */
	if (! (fname = (unsigned char *) NewPtr(28))) {
		mem_fail();
	}

	/* using drive ID, try to find a volume */
	err = GetVInfo(dnum, fname, &vref, &free);
	DisposPtr((Ptr) fname);

	/* if a volume was found, try to unmount it */
	if (err == 0) {
		if (err = UnmountVol(0, vref)) {
			if (err == fBsyErr) {
				/* files are open; common enough we have a dedicated message */
				alert_template(ATYPE_CAUTION, ALRT_GENERIC, STRI_GA_CHNG_BSY);
				return false;
			} else if (err == 0) {
				/* success */
			} else {
				alert_template_error(0, ALRT_GENERIC, STRI_GA_VCHNG_ERR, err);
				return false;
			}
		}
	} else {
		/* assume no matching volume; not necessarily a problem for non-native disks */
	}

	/* tell the driver to eject */
	ctrlp = (CntrlParam *) NewPtrClear(sizeof(CntrlParam));
	if (! ctrlp) {
		mem_fail();
	}
	ctrlp->ioCRefNum = iref;
	ctrlp->csCode = 7; /* csEject? */
	err = PBControl((ParmBlkPtr) ctrlp, 0);
	DisposPtr((Ptr) ctrlp);
	if (err) {
		alert_template_error(0, ALRT_GENERIC, STRI_GA_EJECT_ERR, err);
	} else {
		/* wait for the driver to report ejection clear */
		while (*diskstate >= 0xFC);
	}

	/* finally report back */
	return err == 0;
}

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
Boolean emu_get_info(short item, short *index, long *size)
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
		mem_fail();
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

	return emu_count;
}

/**
 * Mount the selected image. If none is selected, no action is performed.
 *
 * @param scsi_id  the target SCSI ID.
 */
void emu_mount(short scsi_id)
{
	short item, index;
	long err;

	item = 0;
	window_next(&item);

	if (item >= 0 && item < emu_count) {
		index = emu_index[item];
		if (err = scsi_set_image(scsi_id, index)) {
			alert_template_error(0, ALRT_SCSI_ERROR, HiWord(err), LoWord(err));
		} else {
			emu_eject(scsi_id);
			SetCursor(&arrow);
		}
	} else {
		alert_template(0, ALRT_GENERIC, STRI_GA_NSI);
	}
}
