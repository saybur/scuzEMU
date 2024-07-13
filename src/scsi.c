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
#include "scsi.h"
#include "util.h"

/**
 * Implements the SCSI communication with the emulator. The following commands are
 * of interest, shown along with the Toolbox.cpp call:
 *
 * * 0xD0: list files [sharedDir / onListFiles]
 * * 0xD1: get file [sharedDir / onFileGet10]
 * * 0xD2: count files [sharedDir / doCountFiles]
 * * 0xD3: start send [sendFilePrep]
 * * 0xD4: send file block [sendFile10]
 * * 0xD5: end send [sendFileEnd]
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
 */

#define TOOLBOX_MODE_PAGE       0x31
#define TOOLBOX_MODE_PAGE_SIZE  42

/**
 * Low-level general handler for running a transaction against a SCSI target.
 *
 * @param scsi_id  device ID on [0, 6].
 * @param *op      pointer to CDB array to send.
 * @param op_len   length of the CDB array.
 * @param read     true if receiving data from device, false if sending data to device.
 * @param data     address of pointer to save data, if reading, or get data, if sending.
 * @param length   number of data bytes to read, or zero to skip read/write step.
 * @return         error code, or zero for success.
 */
static long scsi_t(short scsi_id, char *op, short op_len, Boolean read, long data, short length)
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

	if (fail = SCSICmd(op, op_len)) {
		/* command failed, need to clear condition */
		fail |= 0x30000;
		goto scsi_t_cleanup;
	}

	if (length > 0) {
		inst[0].scOpcode = scNoInc;
		inst[0].scParam1 = data;
		inst[0].scParam2 = length;
		inst[1].scOpcode = scStop;
		inst[1].scParam1 = 0;
		inst[1].scParam2 = 0;
		if (read) {
			if (fail = SCSIRead((Ptr) &inst)) {
				/* read failed, still need to clean up */
				fail |= 0x40000;
				goto scsi_t_cleanup;
			}
		} else {
			if (fail = SCSIWrite((Ptr) &inst)) {
				/* write failed, still need to clean up */
				fail |= 0x40000;
				goto scsi_t_cleanup;
			}
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

scsi_t_cleanup:
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
 * Performs a simplistic REQUEST SENSE against a SCSI target. The response
 * is stored in condensed format in the given long, with the high 8 bytes 0x00,
 * next 8 the sense key, then the ASC, then the ASCQ.
 *
 * This only supports error code 0x70, if a different code is present this
 * will set the response to -1.
 *
 * @param scsi_id  device ID on [0, 6].
 * @param sense    address of long to recieve the information above.
 * @return         error code, or zero for success.
 */
static long scsi_request_sense(short scsi_id, long *sense)
{
	unsigned char cdb[6];
	unsigned char rs[18];
	long err;

	cdb[0] = 0x03;
	cdb[1] = 0;
	cdb[2] = 0;
	cdb[3] = 0;
	cdb[4] = sizeof(rs);
	cdb[5] = 0;

	if (err = scsi_t(scsi_id, (char *) cdb, sizeof(cdb), true, (long) &rs, sizeof(rs))) {
		*sense = -1;
		return err;
	}

	/* check valid bit and if code is 0x70 */
	if (rs[0] != 0xF0) {
		*sense = -1;
		return 0;
	}

	*sense = ((long) (rs[2] & 0x0F) << 16)
			+ ((long) rs[12] << 8)
			+ (long) rs[13];
	return 0;
}

/**
 * Presents a user Alert related to a SCSI failure.
 *
 * @param fail  the failure code from another function in this unit.
 */
void scsi_alert(long fail)
{
	alert_template_error(0, ALRT_SCSI_ERROR, HiWord(fail), LoWord(fail));
}

/**
 * Fetches the emulator mode page 0x31 from the device and provides the API version
 * being used.
 *
 * Unlike the other functions in this unit, this one will only return an error code if
 * a SCSI communication fault occurs. In all other cases where the mode page cannot be
 * parsed, the failure code will be zero and validity result will be set to false.
 *
 * @param scsi_id  device ID on [0, 6].
 * @param *valid   ture if API version information is valid, false otherwise.
 * @param *ver     the API version being used.
 * @return         fatal error code only, per above.
 */
long scsi_get_emu_api(short scsi_id, Boolean *valid, unsigned char *ver)
{
	char cdb[6];
	long fail, sense;
	char *data;

	/* reserve enough memory for the page and both headers */
	if (! (data = NewPtr(TOOLBOX_MODE_PAGE_SIZE + 6))) {
		mem_fail();
	}

	/*
	 * Do two requests: first is just the required header, to see if there is
	 * a page present and if it has the expected length, then a second request
	 * to get the data itself.
	 */

	cdb[0] = 0x1A;
	cdb[1] = 0x08; /* disable block descriptors */
	cdb[2] = TOOLBOX_MODE_PAGE;
	cdb[3] = 0x00;
	cdb[4] = 4;
	cdb[5] = 0x00;

	/* check if the device can return enough data */
	if (fail = scsi_t(scsi_id, cdb, sizeof(cdb), true, (long) data, 4)) {
		scsi_request_sense(scsi_id, &sense); /* discard result */
		if (fail == 0x40005 || fail >= 0x60000) {
			/*
			 * Either didn't transition to DATA OUT (likely page not implemented)
			 * or ended with CHECK CONDITION for some other reason.
			 */
			fail = 0;
		}
		goto scsi_get_emu_api_fail;
	}
	if (data[0] != TOOLBOX_MODE_PAGE_SIZE + 5) {
		goto scsi_get_emu_api_fail;
	}

	/* ask for that data now */
	cdb[4] = TOOLBOX_MODE_PAGE_SIZE + 6;
	if (fail = scsi_t(scsi_id, cdb, sizeof(cdb), true, (long) data, TOOLBOX_MODE_PAGE_SIZE + 6)) {
		scsi_request_sense(scsi_id, &sense);
		if (fail >= 0x60000) {
			/* this time treat a failure to transition to DATA OUT as fatal */
			fail = 0;
		}
		goto scsi_get_emu_api_fail;
	}

	/*
	 * At present, wiki guidance is to check for an exact match against
	 * the vendor-specific string. If that is updated to something more
	 * universal this can be changed to reflect that.
	 */
	*ver = data[TOOLBOX_MODE_PAGE_SIZE + 5];
	*valid = (*ver == 0x00);

	DisposPtr(data);
	return 0;

scsi_get_emu_api_fail:
	*valid = false;
	DisposPtr(data);
	return fail;
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
 * populated length, if and only if the length below is greater than zero.
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
	long fail, sense;

	scsi_init_cdb(cdb);
	if (open_type) {
		cdb[0] = 0xDA;
	} else {
		cdb[0] = 0xD2;
	}

	if (fail = scsi_t(scsi_id, cdb, sizeof(cdb), true, (long) &data_len, 1)) {
		scsi_request_sense(scsi_id, &sense); /* discard result */
		return fail;
	}

	*length = 40 * data_len;
	if (*length <= 0) return 0;
	if (!(h = NewHandle(*length))) {
		mem_fail();
	}

	if (open_type) {
		cdb[0] = 0xD7;
	} else {
		cdb[0] = 0xD0;
	}

	HLock(h);
	if (fail = scsi_t(scsi_id, cdb, sizeof(cdb), true, (long) *h, *length)) {
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
	long fail, sense;

	scsi_init_cdb(cdb);
	cdb[0] = 0xD1;
	cdb[1] = index;
	cdb[2] = (offset >> 24) & 0xFF;
	cdb[3] = (offset >> 16) & 0xFF;
	cdb[4] = (offset >> 8) & 0xFF;
	cdb[5] = offset & 0xFF;

	if (fail = scsi_t(scsi_id, cdb, sizeof(cdb), true, (long) data, length)) {
		scsi_request_sense(scsi_id, &sense); /* discard result */
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
	long fail, sense;
	short i;

	scsi_init_cdb(cdb);
	cdb[0] = 0xD8;
	cdb[1] = index; /* upgrade if >255 support arrives */

	if (fail = scsi_t(scsi_id, cdb, sizeof(cdb), true, 0, 0)) {
		scsi_request_sense(scsi_id, &sense); /* discard result for now */
		return fail;
	}

	return 0;
}


/**
 * Starts a file upload on the emulator.
 *
 * CDB is 0xD3, remaining bytes are ignored. Must be followed by exactly 33 bytes
 * of null-terminated name data, limited to FAT filename characters, which is the
 * responsibility of the caller.
 *
 * @param scsi_id  the device at the given SCSI ID to command.
 * @param name     the name data as described above.
 * @return         error code, or zero for success.
 */
long scsi_write_start(short scsi_id, unsigned char* name)
{
	char cdb[10];
	long fail, sense;

	scsi_init_cdb(cdb);
	cdb[0] = 0xD3;

	if (fail = scsi_t(scsi_id, cdb, sizeof(cdb), false, (long) name, 33)) {
		scsi_request_sense(scsi_id, &sense); /* discard result */
		return fail;
	}

	return 0;
}

/**
 * Sends an upload file block to the emulator.
 *
 * CDB is 0xD4, two big-endian bytes of data length (max 512), three big-endian
 * bytes indicating the 512-byte block offset to write into, then remaining bytes
 * are ignored.
 *
 * @param scsi_id  the device at the given SCSI ID to command.
 * @param offset   24 bit offset where the block should be saved.
 * @param data     the data to save.
 * @param length   the length of data, max 512.
 * @return         error code, or zero for success.
 */
long scsi_write_block(short scsi_id, long offset, char *data, short length)
{
	char cdb[10];
	long fail, sense;

	if (length > 512) length = 512;

	scsi_init_cdb(cdb);
	cdb[0] = 0xD4;
	cdb[1] = (length >> 8) & 0x03;
	cdb[2] = length & 0xFF;
	cdb[3] = (offset >> 16) & 0xFF;
	cdb[4] = (offset >> 8) & 0xFF;
	cdb[5] = offset & 0xFF;

	if (fail = scsi_t(scsi_id, cdb, sizeof(cdb), false, (long) data, length)) {
		scsi_request_sense(scsi_id, &sense); /* discard result */
		return fail;
	}

	return 0;
}

/**
 * Closes a file upload.
 *
 * CDB is 0xD5, remaining bytes are ignored.
 *
 * @param scsi_id  the device at the given SCSI ID to command.
 * @return         error code, or zero for success.
 */
long scsi_write_end(short scsi_id)
{
	char cdb[10];
	long fail, sense;
	short i;

	scsi_init_cdb(cdb);
	cdb[0] = 0xD5;

	if (fail = scsi_t(scsi_id, cdb, sizeof(cdb), false, 0, 0)) {
		scsi_request_sense(scsi_id, &sense); /* discard result for now */
		return fail;
	}

	return 0;
}
