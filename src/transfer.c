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
 * This compilation unit implements the file transfer logic. This _may_ incorporate
 * a future upload function as well.
 */

#define XFER_BUF_SIZE 4096

/* persist across a full transaction */
static short scsi_id;
static Handle data;
static short *items_ptr;
static short items_cur, items_count, vref;
static Boolean session, error, repl_dup;

/* transaction remaining, for progress tracking; in file blocks */
static long tblks, tprog;

/* updated per file */
static Boolean fopen;
static short fref, findex;
static long fsize, fblk, frem;
static Str63 fname;

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
 * Checks the output directory for file name duplicates. If any are
 * found, the user is asked if they want to overwrite them:
 *
 * - If yes, this will just check that all filenames are OK.
 * - If no, then this will prune out entries with duplicate file names
 *   and not execute a transfer on those files.
 *
 * This needs items_ptr, items_count, and vref set. repl_dup is
 * updated by this call.
 *
 * @return  non-zero of an osErr was raised during the process.
 */
static short transfer_check_duplicates(void)
{
	short i, err;
	Str63 f;
	FInfo fi;
	Boolean user_asked;

	repl_dup = false;
	user_asked = false;

	i = 0;
	while (i < items_count) {

		/* fetch expected user item */
		window_get_item_name(items_ptr[i], f);

		if (err = GetFInfo(f, vref, &fi)) {
			if (err == fnfErr) {
				/* expected, file does not exist, move to next */
				i++;
			} else {
				/* a real error, bail out */
				return err;
			}
		} else {
			/* no error, aka file exists; keep going to check filenames */
			if (! user_asked) {
				repl_dup = CautionAlert(ALRT_DUPLICATES, 0) == 2;
				user_asked = true;
			}

			/*
			 * If the user indicated they want to replace duplicates,
			 * all is fine, we'll let that happen during the file creation
			 * process. If they don't want to replace we need to prune out
			 * duplicate items in the listing.
			 */
			if (repl_dup) {
				i++;
			} else {
				arr_del_short(items_ptr, items_count, i);
				items_count--;
			}
		}
	}

	return 0;
}

/**
 * Handles opening a transfer file for writing. This needs the volume/directory
 * reference pre-set, and will set the per-file variables upon return.
 *
 * If this fails the entire transaction should be halted.
 *
 * @param item  the item number from the list to look up in emu.c.
 * @return      true if open was OK, false otherwise.
 */
static Boolean transfer_file_open(short item)
{
	short err;

	if (!emu_get_info(item, &findex, &fsize)) {
		alert_single(ALRT_GENERIC, 1);
		return false;
	}
	if (!emu_get_info(item, &findex, &fsize)) {
		alert_single(ALRT_GENERIC, 1);
		return false;
	}

	window_get_item_name(item, fname);

	if (err = Create(fname, vref, '????', '????')) {

		if (err == dupFNErr && repl_dup) {

			/* handle by deleting existing file and trying creation again */
			if (err = FSDelete(fname, vref)) {
				transfer_alert_ferr(err);
				return false;
			}
			if (err = Create(fname, vref, '????', '????')) {
				transfer_alert_ferr(err);
				return false;
			}

		} else {
			/* other kind of create error */
			transfer_alert_ferr(err);
			return false;
		}
	}
	if (err = FSOpen(fname, vref, &fref)) {
		transfer_alert_ferr(err);
		return false;
	}

	frem = fsize;
	fblk = 0;

	return true;
}

/**
 * Finishes writing out the current file data and flushes the volume.
 *
 * @return true if successful, false otherwise.
 */
static Boolean transfer_file_close(void)
{
	short err;

	if (! fopen) return false;

	if (err = SetEOF(fref, fsize)) {
		transfer_alert_ferr(err);
		FSClose(fref); /* unconditional, just try to get out */
		return false;
	}
	if (err = FSClose(fref)) {
		transfer_alert_ferr(err);
		return false;
	}
	if (err = FlushVol(0, vref)) {
		transfer_alert_ferr(err);
		return false;
	}

	fopen = false;
	return true;
}

/**
 * Initializes the transfer subsystem.
 */
void transfer_init(void)
{
	session = false;
}

/**
 * Starts a download transaction. This is called when the user starts the
 * process and will do a few things:
 *
 * 1) Invoke a file chooser to select where file(s) are going to be saved.
 * 2) Set up internal memory and state to support the transfer.
 *
 * Call transfer_tick() repeatedly to actually progress with the transfer.
 * See that function for more details.
 *
 * @param scsi   the SCSI ID to work with.
 * @param items  the item number(s) to pull out of the list.
 * @param icnt   set to the number of items in the above list, will be
 *               set to the actual number of items upon non-error return.
 * @return       true if the starting process went OK, false otherwise.
 */
Boolean transfer_start(short scsi, short *items, short *icnt)
{
	Point p;
	SFReply out;
	short i, t, err;
	Boolean dup;

	/* TODO be noiser, this is probably a programming error */
	if (session) return false;

	scsi_id = scsi;
	fopen = false;

	/* store item indexing information */
	if (*icnt <= 0) {
		/* TODO be noiser, this is probably a programming error */
		*icnt = 0;
		return;
	}
	items_count = *icnt;
	items_cur = 0;
	if (!(items_ptr = (short *) NewPtr(items_count * 2))) {
		StopAlert(ALRT_MEM_ERROR, 0);
		return false;
	}
	BlockMove(items, (Ptr) items_ptr, items_count * 2);

	/*
	 * Use a generic placeholder. As a TODO, this should probably use
	 * SFPPutFile and a directory selector instead.
	 */
	SetPt(&p, 20, 20);
	SFPutFile(p, "\pSave File To...", "\p<Here>", 0, &out);
	if (! out.good) {
		goto transfer_start_fail;
	}
	vref = out.vRefNum;

	/* find file collisions, trim if appropriate */
	if (err = transfer_check_duplicates()) {
		transfer_alert_ferr(err);
		goto transfer_start_fail;
	}

	/* calculate the full duration of this transfer */
	tblks = 0;
	tprog = 0;
	for (i = 0; i < items_count; i++) {
		if (!emu_get_info(items_ptr[i], &t, &fsize)) {
			alert_single(ALRT_GENERIC, 1);
			goto transfer_start_fail;
		}
		tblks += fsize / XFER_BUF_SIZE + 1;
	}
	if (tblks < 1) tblks = 1; /* div by 0 safety */

	/* now we are active; reserve memory and track for future */
	if (!(data = NewHandle(XFER_BUF_SIZE))) {
		StopAlert(ALRT_MEM_ERROR, 0);
		goto transfer_start_fail;
	} else {
		session = true;
		*icnt = items_count;
		return true;
	}

/* for failures after index pointer init */
transfer_start_fail:
	DisposPtr((Ptr) items_ptr);
	return false;
}

/**
 * Ends an ongoing transfer session.
 *
 * This only has to be called by others when a transfer is being interrupted
 * early, due to things like a user cancellation, program termination, etc. In
 * all other cases this is called internally. It is still safe to call this
 * anyway.
 */
void transfer_end(void)
{
	if (session) {
		session = false;
		DisposHandle(data);
		DisposPtr((Ptr) items_ptr);
		if (fopen) {
			/* result of error, get rid of partial file */
			FSClose(fref);
			FSDelete(fname, vref);
		}
	}
}

/**
 * Executes transfer block(s).
 *
 * @return  true if transfer ticks should continue, false otherwise.
 */
Boolean transfer_tick(void)
{
	long err, xfer, percent;

	if (! session) return false;

	if (! fopen) {
		/* are there more files to transfer? */
		if (items_cur < items_count
				&& transfer_file_open(items_ptr[items_cur++])) {
			fopen = true;
			progress_set_file(fname);
			progress_set_count(items_count - items_cur + 1);
		} else {
			/* either no more files, or error: in either case stop */
			progress_set_count(0);
			transfer_end();
			return false;
		}
	}

	/* choose size of this transfer tick */
	if (frem < XFER_BUF_SIZE) {
		xfer = frem;
	} else {
		xfer = XFER_BUF_SIZE;
	}
	frem -= xfer;

	/* perform data exchange */
	HLock(data);
	if (err = scsi_read_file(scsi_id, findex, fblk, *data, (short) xfer)) {
		alert_dual(ALRT_SCSI_ERROR, HiWord(err), LoWord(err));
	} else if (err = FSWrite(fref, &xfer, *data)) {
		transfer_alert_ferr(err);
	}
	HUnlock(data);
	if (err) {
		transfer_end();
		return false;
	}
	fblk++;

	tprog++;
	percent = tprog * 100 / tblks;
	progress_set_percent((short) percent);

	if (frem <= 0) {
		if (! transfer_file_close()) {
			transfer_end();
			return false;
		}
	}

	return true;
}
