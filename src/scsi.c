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

#include <SCSI.h>

#include "constants.h"

/**
 * Implements the SCSI communication with the emulator. The following commands are
 * of interest, shown along with the Toolbox.cpp call:
 *
 * * 0xD0: list files [sharedDir / onListFiles]
 * * 0xD1: get file [sharedDir / onFileGet10]
 * * 0xD2: count files [sharedDir / doCountFiles]
 * * 0xD7: list images [CDx / onListFiles]
 * * 0xD8: set next image [onSetNextCD]
 * * 0xD9: list devices [onListDevices] (not implemented as of now)
 * * 0xDA: count images [CDx / doCountFiles]
 *
 * Above is as of 2024-06-15; the command set here may change in the future.
 *
 * TODO: this implementation does not use mode pages or INQUIRY to check
 * compatibility, it is up to the user to select valid devices. It would be
 * nice to handle this in the future.
 *
 * Error reporting from functions in this unit use the following for the high
 * word. Low word is OSErr unless otherwise specified. See the corresponding
 * 'STR#' for what is shown to the user.
 *
 * 0x01: SCSIGet
 * 0x02: SCSISelect
 * 0x03: SCSICmd
 * 0x04: SCSIRead
 * 0x05: SCSIComplete
 * 0x06: status was not COMMAND COMPLETE, in the low word, the high byte is the
 *       message and the low byte is SCSI status
 * 0x07: memory allocation failure
 */

/**
 * "Low level" reading operation against a SCSI target.
 *
 * @param scsi_id  device ID on [0, 6].
 * @param *op      pointer to 10-byte char array of the CDB to send.
 * @param data     address of pointer to write bytes into during reading.
 * @param length   number of data bytes to read, or zero to skip reading.
 * @return         error code, or zero for success.
 */
static long scsi_read(short scsi_id, char *op, long data, short length)
{
	SCSIInstr inst[2];
	long fail;
	short stat, message;

	if (fail = SCSIGet()) {
		/* did not get bus, no cleanup required */
		fail |= 0x10000;
		return fail;
	}

	if (fail = SCSISelect(scsi_id)) {
		/* could not select, no cleanup required */
		fail |= 0x20000;
		return fail;
	}

	if (fail = SCSICmd(op, 10)) {
		/* command failed, need to clear condition */
		fail |= 0x30000;
		goto scsi_read_cleanup;
	}

	if (length > 0) {
		inst[0].scOpcode = scNoInc;
		inst[0].scParam1 = data;
		inst[0].scParam2 = length;
		inst[1].scOpcode = scStop;
		inst[1].scParam1 = 0;
		inst[1].scParam2 = 0;
		if (fail = SCSIRead((Ptr) &inst)) {
			/* read failed, still need to clean up */
			fail |= 0x40000;
			goto scsi_read_cleanup;
		}
	}

	if (fail = SCSIComplete(&stat, &message, 300)) {
		/* completing the command failed, can't fix */
		fail |= 0x50000;
		return fail;
	}

	if (stat != 0) {
		/* not COMMAND COMPLETE */
		fail = 0x60000 | ((message & 0xFF) << 8) | (stat & 0xFF);
		return fail;
	}

scsi_read_cleanup:
	/* try to do a clean hangup */
	SCSIComplete(&stat, &message, SCSI_TIMEOUT);
	return fail;
}

/**
 * Sets 10 CDB bytes to 0. '= {0}' seems to work on Symantec C++ 7, but that's
 * C99 or later? If anyone knows legality here please correct my ignorance!
 */
static void scsi_init_cdb(char *arr)
{
	short i;

	for (i = 0; i < 10; i++) {
		arr[i] = 0;
	}
}

/**
 * Queries a SCSI emulator and asks for a list of available items.
 *
 * To list files requires 2 sequential commands:
 *
 * - CDB 0xD2 for files or 0xD9 for images, remaining CDB ignored. Replies with
 *   1 byte, which is the number of files.
 * - CDB 0xD0 for files or 0xD7 for images, remaining CDB ignored. Replies with
 *   40 bytes for each file: byte 0 is index, 1 is directory yes/no, 2-34 are
 *   filename (C string), 35-39 are size (MSB) with high byte always zeroed.
 *
 * Handle is allocated internally, callers must use DisposHandle to clear.
 *
 * Returns 0 on success. If nonzero, high byte has failure type and low byte has
 * failure code. On success the provided pointer will be set to a new Handle,
 * itself set to a chunk of memory equal to 40 bytes times the size of the
 * populated length.
 *
 * @param scsi_id    device ID on [0, 6].
 * @param open_type  zero for files, non-zero for images
 * @param *data      pointer that will be assigned internally to a new Handle with
 *                   data read from the device.
 * @param *length    length of data above.
 * @return           error code, or zero for success.
 */
long scsi_list_files(short scsi_id, short open_type, Handle *data, short *length)
{
	char cdb[10];
	Handle h;
	unsigned char data_len;
	long fail;

	scsi_init_cdb(cdb);
	if (open_type) {
		cdb[0] = 0xDA;
	} else {
		cdb[0] = 0xD2;
	}

	if (fail = scsi_read(scsi_id, cdb, (long) &data_len, 1)) {
/* FIXME do REQUEST SENSE */
		return fail;
	}

	*length = 40 * data_len;
	if (!(h = NewHandle(*length))) {
		return 0x70001;
	}

	if (open_type) {
		cdb[0] = 0xD7;
	} else {
		cdb[0] = 0xD0;
	}

	HLock(h);
	if (fail = scsi_read(scsi_id, cdb, (long) *h, *length)) {
		/* attempt to read listing failed */
		/* TODO probably should make it clear which call failed */
		HUnlock(h);
		DisposHandle(h);
		return fail;
	}
	*data = h;
	HUnlock(h);

	return 0;
}

/**
 * Reads files from the SCSI emulator.
 *
 * CDB is 0xD1, file_index, then a 32-bit value for the 4K offset within the
 * file to read. Remaining CDB bytes are ignored.
 *
 * @param scsi_id  device ID on [0, 6].
 * @param index    file index from the file listing.
 * @param offset   file offset to read from, in 4K chunks.
 * @param *data    pointer for data read off the SCSI device.
 * @param length   number of bytes to read.
 * @return         0 on success, non-zero on failure.
 */
long scsi_read_file(short scsi_id, short index, long offset, char *data, short length)
{
	char cdb[10];
	long fail;

	scsi_init_cdb(cdb);
	cdb[0] = 0xD1;
	cdb[1] = index;
	cdb[2] = (offset >> 24) & 0xFF;
	cdb[3] = (offset >> 16) & 0xFF;
	cdb[4] = (offset >> 8) & 0xFF;
	cdb[5] = offset & 0xFF;

	if (fail = scsi_read(scsi_id, cdb, (long) data, length)) {
		return fail;
	}

	return 0;
}

/**
 * Asks the emulator to switch to the disk image with the given index.
 *
 * CDB is 0xD8, next byte is the image index to set, remaining bytes are
 * ignored.
 *
 * @param scsi_id  the device at the given SCSI ID to command.
 * @param index    the disk image, from the earlier listing.
 * @return         error code, or zero for success.
 */
long scsi_set_image(short scsi_id, short index)
{
	char cdb[10];
	long fail;

	scsi_init_cdb(cdb);
	cdb[0] = 0xD8;
	cdb[1] = index; /* upgrade if >255 support arrives */

	if (fail = scsi_read(scsi_id, cdb, 0, 0)) {
/* FIXME do REQUEST SENSE */
		return fail;
	}

	return 0;
}
