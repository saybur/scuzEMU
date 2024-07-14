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

#include "list.h"
#include "util.h"

/*
 * This unit contains functions for working with one-dimensional vertical lists.
 */

/**
 * How long, in ticks, must the user idle before the existing typed-in search string
 * is forgotten. This is shorter than what Inside Macintosh seems to recommend (120)
 * but feels more like how a modern UI operates.
 */
#define LIST_TYPING_TIMEOUT  60

static Str15 search;
static long last_search;

/**
 * Deselect everything within a list. Unlike the other functions in this unit this
 * should work on multi-dimensional lists.
 *
 * @param list  the list to perform deselection within.
 */
void list_clear_selections(ListHandle list)
{
	Point p;

	SetPt(&p, 0, 0);
	while (LGetSelect(true, &p, list)) {
		LSetSelect(false, p, list);
	}
}

/**
 * Callback for LSearch that implements the common form of "find the first thing
 * matching this random text string" rather than the default full-text match.
 *
 * This uses internal values as a cache, use care when sharing between lists.
 * The values time out after a short delay so this should not generally be a problem.
 */
pascal short list_cmp(char *cell, char *test, short cl, short tl)
{
	short len, i;
	Boolean match;

	len = cl;
	if (tl < cl) len = tl;

	match = true;
	for (i = 0; i < len; i++) {
		if (lowerc(cell[i]) != lowerc(test[i])) {
			match = false;
			break;
		}
	}

	/* needs inverted logic */
	if (match) {
		return 0;
	} else {
		return 1;
	}
}

/**
 * Handles /inContent/ on /keyDown/ for a vertical list. Main functions:
 *
 * 1) Standard keystrokes will search the list for the first partial match and
 *    highlight it.
 * 2) Arrow keys will navigate the list.
 * 3) Escape will deselect everything.
 *
 * @param list  the list to operate on.
 * @param evt   the event record causing the click.
 */
void list_key(ListHandle list, EventRecord *evt)
{
	char c;
	short count;
	long t;
	Point p, op;
	short is_shift;

	is_shift = evt->modifiers & shiftKey;
	/* hack: if list doesn't allow multiple selection pretend shift is off */
	if ((**list).selFlags & lOnlyOne) {
		is_shift = 0;
	}
	count = (**list).dataBounds.bottom;

	c = evt->message & 0xFF;
	if (c >= 0x20) {
		/* interpret as character */

		/* add to existing text search or not? */
		t = TickCount();
		if (t - last_search > LIST_TYPING_TIMEOUT) {
			search[0] = 1;
			search[1] = c;
			last_search = t;
		} else if (search[0] >= 14) {
			/* no room, ignore; fast typist! (or coding error) */
		} else {
			last_search = t;
			search[++search[0]] = c;
		}

		/* find cell with this name */
		SetPt(&p, 0, 0);
		if (LSearch(&(search[1]), search[0], list_cmp, &p, list)) {
			/* deselect old cells */
			list_clear_selections(list);

			/* select the new cell */
			LSetSelect(true, p, list);
			LAutoScroll(list);
		}
	} else if (c >= 0x1C) {
		/* arrow keys */

		SetPt(&p, 0, 0);
		if (LGetSelect(true, &p, list)) {
			/* existing cell is selected, start from there */
			if (! is_shift) {
				list_clear_selections(list);
			}

			/* shift to new cell position and select it (with roll-over) */
			if (c & 1) {
				/* right/down */
				if (is_shift) {
					/* move to last selected cell before expanding */
					while (LGetSelect(true, &p, list)) {
						p.v++;
					}
				} else {
					p.v++;
				}
				if (p.v >= count) {
					if (is_shift) {
						/* avoid wrapping, it feels weird */
						p.v = count - 1;
					} else {
						p.v = 0;
					}
				}
			} else {
				/* left/up */
				p.v--;
				if (p.v < 0) {
					if (is_shift) {
						p.v = 0;
					} else {
						p.v = count - 1;
					}
				}
			}

			/* if legal, select cell */
			if (p.v >= 0 && p.v < count) {
				LSetSelect(true, p, list);
			}
		} else {
			/* just select the first list entry */
			LSetSelect(true, p, list);
		}
		LAutoScroll(list);
	} else if (c == 0x1B) {
		/* esc/clear */
		list_clear_selections(list);
	}
}

/**
 * Provides the next selected item number within the vertical list, starting with the
 * given item.
 *
 * This is just a wrapper around LGetSelect(true), so it can be called repeatedly,
 * starting at 0, returning the next selected cell each time (though don't forget
 * to add 1 to the result to move past the last-selected cell).
 *
 * @param list  the list to operate on.
 * @param *i    on input, the cell to start at; will be set to the next selected
 *              cell, or -1 if there are no more cells.
 */
void list_next(ListHandle list, short *i)
{
	Point p;

	p.h = 0;
	p.v = *i;
	if (LGetSelect(true, &p, list)) {
		*i = p.v;
	} else {
		*i = -1;
	}
}
