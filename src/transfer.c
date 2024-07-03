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
#include "emu.h"
#include "window.h"
#include "util.h"

/*
 * This implements the file transfer logic. This _may_ incorporate a future upload
 * function as well. Only one file at a time is currently supported but this approach
 * could potentially allow for multi-file downloads as well with appropriate changes.
 */

static short scsi_id;
static Handle fdata;
static Boolean is_open;
static short index, fref, vref;
static long fsize, fblk, frem;

/**
 * Shows an appropriate alert when a file error occurs.
 *
 * @param err the OSErr triggering the alert.
 */
static void transfer_alert_ferr(short err)
{
	short esi;

	/* fetch appropriate STR# for the error and show it */
	esi = (err + 31) * -1;
	if (esi < 1 || esi > 30) {
		esi = 1;
	}
	alert_dual(ALRT_FILE_ERROR, esi, err);
}

/**
 * Starts a download. This is called when the user starts the process and will do
 * a few things:
 *
 * 1) Invoke a file chooser to select where file(s) are going to be saved.
 * 2) Set up internal memory and state to support the transfer.
 * 3) Invoke a moveable modal dialog to show the status of the transfer.
 *
 * This will then post an app3Evt and return.
 *
 * @param scsi  the SCSI ID to work with.
 * @param item  the item number to pull out of the list.
 * @return      true if the starting process went OK, false otherwise.
 */
Boolean transfer_start(short scsi, short item)
{
	Point p;
	Str255 s;
	short l, err;
	SFReply out;

	scsi_id = scsi;
	is_open = false;

	if (!emu_get_info(item, &index, &fsize)) {
		alert_single(ALRT_GENERIC, 1);
		return false;
	}
	frem = fsize;
	fblk = 0;

	if (!(fdata = NewHandle(4096))) {
		StopAlert(ALRT_MEM_ERROR, 0);
		return false;
	}

	/* get name of item in list for the save dialog default */
	window_get_item_name(item, s);

	/* present the save dialog; if they approve, set up the transfer */
	SetPt(&p, 20, 20);
	SFPutFile(p, "\pSave File To...", s, 0, &out);
	if (out.good) {
		busy_cursor();

		vref = out.vRefNum;

		if (err = Create(out.fName, vref, '????', '????')) {

			/* allow for duplicate file error; user should have seen and OK'd this */
			if (err == dupFNErr) {

				/* handle by deleting existing file and trying creation again */
				if (err = FSDelete(out.fName, vref)) {
					transfer_alert_ferr(err);
					return false;
				}
				if (err = Create(out.fName, vref, '????', '????')) {
					transfer_alert_ferr(err);
					return false;
				}

			} else {
				/* other kind of create error */
				transfer_alert_ferr(err);
				return false;
			}
		}
		if (err = FSOpen(out.fName, vref, &fref)) {
			transfer_alert_ferr(err);
			return false;
		}
		is_open = true;

		return true;
	} else {
		return false;
	}
}


/**
 * @return  the current transfer progress as a value from 0 to 100 (ish).
 */
short transfer_progress(void)
{
	if (fsize) {
		return (fsize - frem) * 100 / fsize;
	} else {
		return 0;
	}
}

/**
 * Executes transfer block(s).
 *
 * @return  true if transfer ticks should continue, false otherwise.
 */
Boolean transfer_tick(void)
{
	long err, xfer;

	if (frem < 4096) {
		xfer = frem;
	} else {
		xfer = 4096;
	}
	frem -= xfer;

	HLock(fdata);
	if (err = scsi_read_file(scsi_id, index, fblk, *fdata, (short) xfer)) {
		alert_dual(ALRT_SCSI_ERROR, HiWord(err), LoWord(err));
	} else if (err = FSWrite(fref, &xfer, *fdata)) {
		transfer_alert_ferr(err);
	}
	HUnlock(fdata);

	fblk++;

	if (err) {
		return false;
	} else if (frem <= 0) {
		return false;
	} else {
		return true;
	}
}

/**
 * Called to end a transfer, closing the file locally and discarding memory.
 */
void transfer_end(void)
{
	short err;

	if (fdata) {
		DisposHandle(fdata);
	}
	if (is_open) {
		is_open = false;
		if (err = SetEOF(fref, fsize)) {
			transfer_alert_ferr(err);
			FSClose(fref); /* unconditional, just try to get out */
			return;
		}
		if (err = FSClose(fref)) {
			transfer_alert_ferr(err);
			return;
		}
		if (err = FlushVol(0, vref)) {
			transfer_alert_ferr(err);
			return;
		}
	}
}
