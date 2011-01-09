/* This file is part of Vidad.
*
*  Vidad is free software: you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  Vidad is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with Vidad.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <kernel.h>

/* TODO:
* write specific info about the exceptions, but only after bootloader rewrite, much strings.
*/

/* array to store callbacks */
dword inter_callback[16][NUM_ENTRIES][2];

/* initialise interrupt handling code */
void inter_init()
{	int i, j;
	for (i=0; i<16; i++)
	{	for (j=0; j<NUM_ENTRIES; j++)
		{	inter_callback[i][j][0] = inter_callback[i][j][1] = 0;
		}
	}
}

/* called by handle_int */
void inter_main(byte int_num)
{	dword i;
	if (int_num == 0x00) kerror("Exception 0x00, divide by zero, sorry mate, we cannot do that.", 0x4F);
	if (int_num == 0x01) kerror("Exception 0x01, who the heck turned the debug flag on?!", 0x4F);
	if (int_num == 0x02) kerror("Exception 0x02, non-maskable interrupt", 0x4F);
	if (int_num == 0x03) kerror("Exception 0x03, breakpoint, now, what did you want to do with it?", 0x4F);
	if (int_num == 0x04) kerror("Exception 0x04, overflow", 0x4F);
	if (int_num == 0x05) kerror("Exception 0x05, bound range exceeded", 0x4F);
	if (int_num == 0x06) kerror("Exception 0x06, invalid opcode, aka, you're executing data.", 0x4F);
	if (int_num == 0x07) kerror("Exception 0x07, device not available, this machine has no FPU, you got to be kidding, when did you buy it, 1985?", 0x4F);
	if (int_num == 0x08) kerror("Exception 0x08, double fault, this is not good!", 0x4F);
	if (int_num == 0x09) kerror("Exception 0x09, coprocessor segment overrun, god knows what this means.", 0x4F);
	if (int_num == 0x0A) kerror("Exception 0x0A, invalid TSS, but, but, we aren't using those!", 0x4F);
	if (int_num == 0x0B) kerror("Exception 0x0B, segment not present", 0x4F);
	if (int_num == 0x0C) kerror("Exception 0x0C, stack segment fault", 0x4F);
	if (int_num == 0x0D) kerror("Exception 0x0D, general protection fault, aka, pointer error.", 0x4F);
	if (int_num == 0x0E) kerror("Exception 0x0E, page fault, OK, whose idea was it to turn on paging again?", 0x4F);
	if (int_num == 0x10) kerror("Exception 0x10, x87 floating point exception (floats are horrible things, not?)", 0x4F);
	if (int_num == 0x11) kerror("Exception 0x11, alignment check", 0x4F);
	if (int_num == 0x12) kerror("Exception 0x12, machine check", 0x4F);
	if (int_num == 0x13) kerror("Exception 0x13, SIMD floating-point exception", 0x4F);
	if (int_num == 0x1E) kerror("Exception 0x1E, security exception", 0x4F);
	/* routine for unhandled EXCEPS, 0x00-0x1F */
	if (int_num < 0x20) kerror("Unhandled Exception", 0x4F);
	if (int_num < 0x30) /* IRQ's */
	{	for (i=0; i<NUM_ENTRIES; i++)
		{	if (! inter_callback[int_num - 0x20][i][0]) continue;
			(*((void (*)(dword)) inter_callback[int_num-32][i][0]))(inter_callback[int_num-32][i][1]);
		}
		if (int_num >= 0x28) outportb(0x00A0, 0x20);
		outportb(0x0020, 0x20);
		return;
	}
	/* routine for unhandled INTS, 0x30-0x3F */
	kerror("Unhandled interrupt", 0x2F);
}

/* register a callback function */
int inter_reg(dword addr, dword obj, dword irq)
{	dword i = 0;
	if (irq > 16) return(0);
	while (inter_callback[irq][i][0]) { i++; if (i == NUM_ENTRIES) return(-1); }
	inter_callback[irq][i][0] = addr;
	inter_callback[irq][i][1] = obj;
	return(i);
}

/* de-register a callback function */
void inter_dereg(dword irq, int num)
{	inter_callback[irq][num][0] = 0;
}
