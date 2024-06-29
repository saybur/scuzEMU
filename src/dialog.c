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

/**
 * Handles drawing a dot pattern across elements of a dialog.
 *
 * @param w  the window to apply against.
 * @param i  the item number to draw into.
 */
pascal static void draw_dots(WindowPtr w, short i)
{
	short itype;
	Handle ihandle;
	Rect irect;
	GrafPtr og;

	GetDItem(w, i, &itype, &ihandle, &irect);
	GetPort(&og);
	SetPort(w);

	FillRect(&irect, &(qd.gray));

	SetPort(og);
}

/**
 * Handles drawing the 3 pixel border around the default button for a dialog.
 * This is (apparently) a common idiom for Mac programs. See THINK Reference
 * ModalDialog for an example of usage.
 *
 * @param w  the window to apply against.
 * @param i  the item number to draw into.
 */
pascal static void draw_default_border(WindowPtr w, short i)
{
	short itype;
	Handle ihandle;
	Rect irect;
	GrafPtr og;
	PenState op;
	long bo;

	GetDItem(w, 1, &itype, &ihandle, &irect);
	GetPort(&og);
	SetPort(w);
	GetPenState(&op);

	PenNormal();
	InsetRect(&irect, -4, -4);
	FrameRoundRect(&irect, 16, 16);
	bo = (irect.bottom - irect.top) / 2 + 2;

	PenPat(&(qd.black));
	PenSize(3, 3);
	FrameRoundRect(&irect, bo, bo);

	SetPenState(&op);
	SetPort(og);
}

/**
 * Presents a modal dialog asking the user to enter the SCSI ID and whether to open a
 * file or image list. Returns true if the user pressed "OK," thus updating the input
 * values.
 *
 * @param scsi       SCSI ID, from 0-6.
 * @param open_type  either 0 for files or 1 for images.
 * @return           true if user selected OK, false otherwise.
 */
Boolean dialog_open(short *scsi, short *open_type)
{
	DialogPtr dialog;
	short item_hit, item_type, sel_scsi, sel_open_type;
	Handle item_handle;
	Rect rect;

	dialog = GetNewDialog(DLOG_OPEN, 0L, (WindowPtr) -1);
	if (dialog) {

		/* assign separator drawing code */
		GetDItem(dialog, 14, &item_type, &item_handle, &rect);
		SetDItem(dialog, 14, item_type, (Handle) draw_dots, &rect);

		/* assign the default item border drawing code */
		GetDItem(dialog, 15, &item_type, &item_handle, &rect);
		SetDItem(dialog, 15, item_type, (Handle) draw_default_border, &rect);

		/* SCSI radios are 3-9 for IDs 0-6; unsel last, sel new */
		GetDItem(dialog, *scsi + 3, &item_type, &item_handle, &rect);
		SetCtlValue((ControlHandle) item_handle, 1);
		sel_scsi = *scsi;

		/* open type is 10 for files and 11 for images */
		GetDItem(dialog, *open_type + 10, &item_type, &item_handle, &rect);
		SetCtlValue((ControlHandle) item_handle, 1);
		sel_open_type = *open_type;

		ShowWindow(dialog);
		do {
			/* wait for click */
			ModalDialog(0L, &item_hit);

			if (item_hit >= 12) {
				continue;
			} else if (item_hit >= 10) {
				GetDItem(dialog, sel_open_type + 10, &item_type, &item_handle, &rect);
				SetCtlValue((ControlHandle) item_handle, 0);
				sel_open_type = item_hit - 10;
				GetDItem(dialog, item_hit, &item_type, &item_handle, &rect);
				SetCtlValue((ControlHandle) item_handle, 1);
			} else if (item_hit >= 3) {
				GetDItem(dialog, sel_scsi + 3, &item_type, &item_handle, &rect);
				SetCtlValue((ControlHandle) item_handle, 0);
				sel_scsi = item_hit - 3;
				GetDItem(dialog, item_hit, &item_type, &item_handle, &rect);
				SetCtlValue((ControlHandle) item_handle, 1);
			}
		} while (item_hit >= 3);
		DisposDialog(dialog);

		/* if user OK'd, update from indicated radio */
		if (item_hit == 1) {
			*scsi = sel_scsi;
			*open_type = sel_open_type;
			return true;
		}
	} else {
		StopAlert(ALRT_MEM_ERROR, 0);
	}

	return false;
}
