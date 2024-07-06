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

#ifndef __CONSTANTS__
#define __CONSTANTS__

/*
 * ----------------------------------------
 *     PROGRAM DETAILS
 * ----------------------------------------
 */

#define WAIT_EVENT_SLEEP    30
#define SCSI_TIMEOUT        300
#define WINDOW_MIN_HEIGHT   200
#define WINDOW_MIN_WIDTH    240

/* This is more than what is actually allowed (100), here for possible future changes */
#define MAXIMUM_FILES       255

/*
 * ----------------------------------------
 *     RESOURCE IDS
 * ----------------------------------------
 */

#define ALRT_ABOUT          128
#define ALRT_IMAGE_CHANGE   129
#define ALRT_NO_IMAGES      130
#define ALRT_NO_FILES       131
#define ALRT_DUPLICATES     132
#define ALRT_IMG_NO_DEV     133
#define ALRT_IMG_CHNG_BSY   134
#define ALRT_IMG_VCHNG_ERR  135
#define ALRT_IMG_EJECT_ERR  136
#define ALRT_GENERIC        256
#define ALRT_BAD_VERSION    257
#define ALRT_SCSI_ERROR     258
#define ALRT_FILE_ERROR     259
#define ALRT_MEM_ERROR      260

#define CNTL_STOP           128

#define DLOG_OPEN           512

#define MENU_APPLE          128
#define MENU_FILE           129
#define MENU_EDIT           130

#define STR_GENERAL         128

#define WIND_MAIN           128
#define WIND_PROGRESS       129

/*
 * ----------------------------------------
 *     STRING INDEXES
 * ----------------------------------------
 */

#define STRI_GEN_FILES      1
#define STRI_GEN_IMAGES     2
#define STRI_GEN_DOWNLOAD   3
#define STRI_GEN_COPYING    4
#define STRI_GEN_WRITING    6

#endif /* __CONSTANTS__ */
