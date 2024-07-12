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

#include "types.h"

/*
 * FIXME these probably belong in the resources instead.
 */
#define BINHEX_MAGIC  "(This file must be converted with BinHex 4.0)"
#define SIT_MAGIC     "rLau"
#define SIT5_MAGIC    "Stuff"

/**
 * Get the suffix of the given Pascal filename string as a numeric, lower-case
 * number, if available.
 *
 * @param name  pointer to the Pascal name to parse.
 * @return      the suffix as a long, or 0 if it couldn't be parsed.
 */
static long suffix_id(unsigned char *name)
{
	short i, len, t;
	long v;
	char c;

	/* sanity check */
	if (! name) {
		return 0;
	}
	len = name[0];
	if (len < 4) {
		return 0;
	}

	/* find the last dot, if one exists */
	for (i = len; i >= 1; i--) {
		if (name[i] == '.') break;
	}

	/* see if it is legal */
	t = len - i;
	if (! (t == 3 || t == 4)) {
		return 0;
	}

	/* then process */
	v = 0;
	t = 0;
	for (i = 0; i < 4; i++) {
		c = name[len - i];
		/* poor-man's tolower() */
		if (c >= 'A' && c <= 'Z') {
			c += 0x20;
		}
		v |= ((long) c << t);
		t += 8;
	}
	return v;
}

/**
 * Quick and dirty memcmp() to avoid importing the ANSI libraries. Is there a Toolbox
 * call that can do this?
 *
 * @param a    first block to compare.
 * @param b    second block to compare.
 * @param len  length of the data to compare.
 * @return     true if equal, false if not.
 */
static Boolean is_eql(char *a, const char *b, short len)
{
	short i;

	for (i = 0; i < len; i++) {
		if (a[i] != b[i]) return false;
	}
	return true;
}

/**
 * Try to find a suitable file type and creator for a file, given the first block
 * or so of data (for magic numbers) and the filename.
 *
 * Inferring a file type/creator is not really an exact science, particularly choosing
 * the creator application; this isn't interested in trying to capture all possibilities,
 * it's just trying to get the most common types of files that would likely be downloaded
 * from a memory card off a SCSI emulator (and work around some common apps being annoying
 * about opening files without the right types). If there are type/creator pairs that fit
 * that description please open a PR or issue with suggestions for changes.
 *
 * As of July 2024 this is pretty underbaked, it mostly just defers to Stuffit Expander.
 *
 * @param data     pointer to the first data block of the file, 512 bytes at minimum.
 * @param name     pointer to the Pascal name of the file.
 * @param type     address to the long that will receive the found file type.
 * @param creator  address to the long that will receive the found file creator.
 */
void types_find(char *data, unsigned char *name, long *type, long *creator)
{
	short nl;
	long suffix;

	*type = '????';
	*creator = '????';

	suffix = suffix_id(name);

	/* Mac Binary */
	if (suffix == '.bin'
			&& data[0] == 0x00
			&& data[74] == 0x00) {
		*type = 'BINA';
		*creator = 'SITx';
		return;
	}

	/* BinHex 4 */
	if (suffix == '.hqx'
			&& is_eql(data, BINHEX_MAGIC, sizeof(BINHEX_MAGIC) - 1)) {
		*type = 'TEXT';
		*creator = 'SITx';
		return;
	}

	/* Stuffit <5 */
	if (suffix == '.sit'
			&& is_eql(data, "SIT!", 4)
			&& is_eql(&(data[10]), SIT_MAGIC, sizeof(SIT_MAGIC) - 1)) {
		/*
		 * Just a guess, but the version field mentioned in the Unarchiver
		 * documentation seems to line up with the difference between SIT! files
		 * for 1.5.1 and later SITD files for 3.5+; we'll assume that here, but
		 * please correct me if I'm wrong.
		 */
		*creator = 'SITx';
		if (data[15] == 0x01) {
			*type = 'SIT!';
			return;
		} else {
			*type = 'SITD';
			return;
		}
	}

	/* Stuffit 5+ */
	if (suffix == '.sit'
			&& is_eql(data, SIT5_MAGIC, sizeof(SIT5_MAGIC) - 1)) {
		*type = 'SIT5';
		*creator = 'SITx';
		return;
	}
}
