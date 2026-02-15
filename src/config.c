/*
 * Copyright (C) 2024-2026 saybur
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
#include "config.h"
#include "scsi.h"
#include "util.h"

Boolean g_use_wne;
Boolean g_use_qdcolor;

static unsigned char mode_checked;
static unsigned char capabilities[8];

/**
 * Reads the 0x31 mode page and sets version information appropriately.
 *
 * The philosophy of this project is to work with anything implementing the toolbox,
 * so this "check" is mostly to verify we're speaking roughly the same language as the
 * device is expecting. If it doesn't appear to be an emulator this will ask the user
 * if they want to continue, remembering the result to avoid pestering them on
 * subsequent connections.
 *
 * As of July 2024 this check is mostly pointless, the only toolbox API is v0, but in
 * future releases there will likely be a need to get/set a version to communicate
 * properly.
 *
 * As of February 2026 there is a kind of 'version 0+' with variable transfer length
 * support. 0xD9 has been modified to return a 'capabilities' flag indicating whether
 * this support is present. This function will query that command to look for the new
 * status information; see scsi.c for the mechanism.
 *
 * @param scsi  SCSI ID to check against, between 0-6.
 * @return      true if connecting should continue, false otherwise.
 */
Boolean config_check_mode(short scsi)
{
	Boolean valid;
	long err;
	unsigned char ver, mask;

	if (scsi < 0 || scsi > 6) return false;
	mask = 1 << scsi;

	/* have we already checked? */
	if (mode_checked & mask) {
		/*
		 * For the moment treat this as an unconditional OK; if
		 * the API has breaking changes at v0 this will need to
		 * be revisited.
		 */
		return true;
	}

	err = scsi_get_emu_api(scsi, &valid, &ver);

	/* resolve errors that prevent a connection from being made */
	if (err) {
		/* note: by design this does not stop subsequent attempts */
		scsi_alert(err);
		return false;
	}

	/* check if this seems to be a device we can talk to */
	if (valid && ver == 0x00) {
		mode_checked |= mask;
	} else {
		if (CautionAlert(ALRT_EMU_MODEPAGE, 0) == 1) {
			/* user is OK trying anyway, don't ask again */
			mode_checked |= mask;
			valid = true;
		} else {
			valid = false;
		}
	}

	/* load the capabilities information for this device */
	if (valid) {
		/* deliberately ignore errors here */
		scsi_get_emu_capabilities(scsi, &(capabilities[scsi]));
	}

	return valid;
}

/**
 * Uses the post-Feb 2026 capabilities information to check if newer features
 * are available.
 *
 * @param scsi     SCSI ID to check against, between 0-6.
 * @param feature  feature flag, see header for definitions.
 * @return         true if capability is available, false otherwise.
 */
Boolean config_has_capability(short scsi, short feature)
{
	if (scsi < 0 || scsi > 6) return false;
	return capabilities[scsi] & feature;
}

/**
 * Called at startup to initialize any relevant globals.
 */
void config_init(void)
{
	long gr;

	mode_checked = 0;

	if (! trap_available(_Gestalt)) {
		g_use_wne = false;
		g_use_qdcolor = false;
		return;
	}

	g_use_wne = trap_available(_WaitNextEvent);

	if (! Gestalt(gestaltQuickdrawFeatures, &gr)) {
		g_use_qdcolor = gr & (1 << gestaltHasColor);
	} else {
		g_use_qdcolor = false;
	}
}
