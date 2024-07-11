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
#include "progress.h"
#include "scsi.h"
#include "util.h"

#define UPLOAD_BUF_SIZE 512

static short scsi_id;
static Handle data;
static Boolean fopen;
static short fref;
static long fsize, fblk, frem;

/**
 * Shows an appropriate alert when a file error occurs.
 *
 * @param err the OSErr triggering the alert.
 */
static void upload_alert_ferr(short err)
{
	short esi;

	/* get appropriate STR# index for osErr */
	esi = (err + 31) * -1;
	if (esi < 1 || esi > 30) {
		esi = 1;
	}

	alert_template_error(0, ALRT_FILE_ERROR, esi, err);
}

/**
 * Checks a given string to see if the characters are allowed on the remote filesystem.
 *
 * This is likely more restrictive than the device, allowing only the fully portable
 * POSIX filename characters from Wikipedia.
 *
 * @param str  the Pascal string to check.
 * @return     true if OK, false otherwise.
 */
static Boolean upload_check_name(unsigned char *name)
{
	short i;
	char c;

	if (! name) return false;
	for (i = 1; i <= name[0]; i++) {
		c = name[i];
		if (! ((c > 48 && c <= 57) /* 0-9 */
				|| (c > 65 && c <= 90) /* A-Z */
				|| (c > 97 && c <= 122) /* a-z */
				|| c == '_'
				|| c == '-'
				|| c == '.')) {
			return false;
		}
	}
	return true;
}

/**
 * Initializes the upload subsystem.
 */
void upload_init(void)
{
	fopen = false;
}

/**
 * Called when a user requests an upload. This will invoke the file chooser, get
 * the picked file, verify it will be OK (hopefully) to transfer, pop up the progress
 * bar, and start the file transfer process. As with the other subsystem, if this
 * returns true the process started OK: call tick() repeatedly until it returns false.
 *
 * @param scsi  the SCSI ID to send the file to.
 * @return      true if the process started OK, false otherwise.
 */
Boolean upload_start(short scsi)
{
	Point p;
	SFReply reply;
	unsigned char *name;
	short i, nl;
	long err;
	char c;

	if (fopen) return false;

	scsi_id = scsi;
	fopen = false;

	/* let the user pick out the file */
	SetPt(&p, 20, 20);
	SFGetFile(p, 0, 0, -1, 0, 0, &reply);
	if (! reply.good) {
		return false;
	}

	/* open it */
	if (err = FSOpen(reply.fName, reply.vRefNum, &fref)) {
		upload_alert_ferr(err);
		return false;
	}

	/* store information about the file length */
	if (err = GetEOF(fref, &fsize)) {
		upload_alert_ferr(err);
		FSClose(fref);
		return false;
	}
	frem = fsize;
	fblk = 0;

	/* convert the file name to what the emulator expects */
	if (reply.fName[0] > 32) {
		alert_template(0, ALRT_GENERIC, STRI_GA_UP_BADLEN);
		goto upload_start_fail;
	}
	if (! upload_check_name(reply.fName)) {
		alert_template(0, ALRT_GENERIC, STRI_GA_UP_BADCHAR);
		goto upload_start_fail;
	}
	if (! (name = (unsigned char *) NewPtrClear(33))) {
		mem_fail();
	}
	BlockMove(&(reply.fName[1]), name, reply.fName[0]);

	/* open the file on the remote device */
	err = scsi_write_start(scsi_id, name);
	DisposPtr((Ptr) name);
	if (err) {
		alert_template_error(0, ALRT_SCSI_ERROR, HiWord(err), LoWord(err));
		goto upload_start_fail;
	}

	/* allocate a buffer for the operation */
	if (! (data = NewHandle(UPLOAD_BUF_SIZE))) {
		mem_fail();
	}

	/* progression reporting setup */
	progress_set_file(reply.fName);
	progress_set_count(1);
	progress_set_percent(0);

	/* all set up */
	fopen = true;
	return true;

upload_start_fail:
	FSClose(fref);
	return false;
}

/**
 * Ends the upload process by freeing resources and closing both the remote
 * and local file.
 *
 * This only needs to be called if closing the upload unexpectedly. If the ticks() call
 * returns false this will already have been invoked. This is safe to call in any event,
 * it will only execute a close if items are already open.
 */
void upload_end(void)
{
	long err;

	if (! fopen) return;

	DisposHandle(data);

	/* close up; at this point errors can't really be resolved, just alert the user */
	if (err = scsi_write_end(scsi_id)) {
		alert_template_error(0, ALRT_SCSI_ERROR, HiWord(err), LoWord(err));
	}
	if (err = FSClose(fref)) {
		upload_alert_ferr(err);
	}
	fopen = false;
}

/**
 * Executes upload block(s). If this returns false,
 *
 * @return  true if upload ticks should continue, false otherwise.
 */
Boolean upload_tick(void)
{
	long err, xfer;

	if (! fopen) return false;

	/* choose size of this upload tick */
	if (frem < UPLOAD_BUF_SIZE) {
		xfer = frem;
	} else {
		xfer = UPLOAD_BUF_SIZE;
	}
	frem -= xfer;

	if (xfer == 0) {
		progress_set_percent(100);
		upload_end();
		return false;
	} else {
		progress_set_percent(fblk * 100 / (fsize / UPLOAD_BUF_SIZE + 1));

		/* send the data block */
		HLock(data);
		if (err = FSRead(fref, &xfer, *data)) {
			upload_alert_ferr(err);
		} else if (err = scsi_write_block(scsi_id, fblk, *data, (short) xfer)) {
			alert_template_error(0, ALRT_SCSI_ERROR, HiWord(err), LoWord(err));
		}
		HUnlock(data);
		fblk++;

		if (err) {
			upload_end();
			return false;
		} else {
			return true;
		}
	}
}
