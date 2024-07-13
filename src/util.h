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

#ifndef __UTILH__
#define __UTILH__

/**
 * A generic "I've run out of memory" message used to inform the user that the heap
 * has become exhausted and the program will shortly terminate.
 */
#define ALRT_UTIL_MEM_FAIL   1000

/**
 * Set of options for the generic alert dialog box. Unrecognized values use
 * a per-call default.
 */
#define ATYPE_NOTE           1
#define ATYPE_CAUTION        2
#define ATYPE_STOP           3

void alert_template(short type, short res_id, short str_id);
void alert_template_error(short type, short res_id, short str_id, short err);
void arr_del_short(short *arr, short len, short itm);
void busy_cursor(void);
void center_window(WindowPtr window);
Boolean init_is_wne(void);
Boolean init_program(void (*quit)(void), short ptrcnt);
void mem_fail(void);
void repl_caret(unsigned char *s);
Boolean str_eq(char *a, const char *b, short len);
void str_load(short id, short idx, unsigned char *str, short size);

#endif /* __UTILH__ */
