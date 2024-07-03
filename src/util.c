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

static Str255 es0;
static Str63 es1;

/**
 * Presents a formatted StopAlert to the user with a message.
 *
 * This needs an 'ALRT' and 'STR#' with the same resource ID, which will be shown with the
 * given string ID as "^0". This also resets the cursor to an arrow.
 *
 * @param res_id  the resource to pull.
 * @param str_id  the string within the 'STR#' to insert.
 */
void alert_single(short res_id, short str_id)
{
	SetCursor(&arrow);
	GetIndString(es0, res_id, str_id);
	ParamText(es0, 0, 0, 0);
	StopAlert(res_id, 0L);
}

/**
 * Presents a formatted StopAlert to the user with a message and an error code for later
 * debugging.
 *
 * This needs an 'ALRT' and 'STR#' with the same resource ID, which will be shown with the
 * given string ID as "^0" and the given code as "^1". This also resets the cursor to an
 * arrow.
 *
 * @param res_id  the resource to pull.
 * @param str_id  the string within the 'STR#' to insert.
 * @param code    the error code to format and add.
 */
void alert_dual(short res_id, short str_id, short code)
{
	SetCursor(&arrow);
	GetIndString(es0, res_id, str_id);
	NumToString(code, es1);
	ParamText(es0, es1, 0, 0);
	StopAlert(res_id, 0L);
}

static Cursor busy_curs;

/**
 * One-line call to change the cursor to show the stopwatch symbol.
 */
void busy_cursor(void)
{
	SetCursor(&busy_curs);
}

/**
 * Sets up utility function memory. Should be called once during program init.
 */
void util_init(void)
{
	CursHandle h;

	h = GetCursor(watchCursor);
	if (h) {
		HLock((Handle) h);
		BlockMove(*h, &busy_curs, sizeof(Cursor));
		HUnlock((Handle) h);
		DisposHandle((Handle) h);
	}
}

/**
 * Handles loading length-limited strings from resources. Use when you want a STR# entry
 * but don't have a guaranteed 256 bytes of final storage available.
 *
 * @param list_id  the STR# to load.
 * @param index    the index within the string list.
 * @param *tmp     pointer to 256 bytes of temporary storage.
 * @param *str     pointer to the resulting string storage.
 * @param size     the data size of the above storage.
 */
void str_load(short list_id, short index, unsigned char *tmp, unsigned char *str, short size)
{
	short len;

	GetIndString(tmp, list_id, index);
	if (tmp[0] > size - 1) {
		tmp[0] = size - 1;
	}

	if (tmp[0] > 0) {
		BlockMove(tmp, str, tmp[0] + 1);
	}
}
