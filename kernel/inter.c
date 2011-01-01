#include "helper.h"
#include "kprint.h"

extern void kerror(char *str, byte color);

#define NUM_ENTRIES 2

/* TODO:
* write specific info about the exceptions, but only after bootloader rewrite, much strings.
*/

/* array to store callbacks */
dword inter_callback[16][NUM_ENTRIES][2];

/* called by handle_int */
void inter_main(byte int_num)
{	dword i;
	/* routine for unhandled EXCEPS, 0x00-0x1F */
	if (int_num < 0x20) kerror("Unhandled Exception", WHITE_RED);
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
	kerror("Unhandled interrupt", WHITE_GREEN);
};

/* register a callback function */
int inter_reg(dword addr, dword obj, dword irq)
{	dword i = 0;
	if (irq > 16) return(0);
	while (inter_callback[irq][i][0]) { i++; if (i == NUM_ENTRIES) return(-1); }
	inter_callback[irq][i][0] = addr;
	inter_callback[irq][i][1] = obj;
	return(i);
};

/* de-register a callback function */
void inter_dereg(dword irq, int num)
{	inter_callback[irq][num][0] = 0;
};
