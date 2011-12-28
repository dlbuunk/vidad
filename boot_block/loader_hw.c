//      loader_hw.c
//      
//      Copyright 2011 D.L.Buunk <dlbuunk@gmail.com>
//      
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//      
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//      
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
//      MA 02110-1301, USA.

// General typedefs
typedef unsigned char byte;
typedef unsigned short int word;
typedef unsigned int dword;

// Common inb/outb functions
void outb(byte val, word port)
{
	asm volatile (
		"outb	%%al,%%dx\n\t"
		:
		: "a" (val)
		, "d" (port)
		:
	);
}

byte inb(word port)
{
	byte val;
	asm volatile (
		"inb	%%dx,%%al\n\t"
		: "=a" (val)
		: "d" (port)
		:
	);
	return val;
}

// Screen functions
word cursor_pos;

void putchar(char ch)
{
	if (ch == '\n')
		cursor_pos = (cursor_pos/80 + 1) * 80;
	else if (ch == '\t')
		cursor_pos = ((cursor_pos>>3) + 1) << 3;
	else
		*((word *)(0x000B8000 + (cursor_pos++ <<1))) = 0x0700 | ch;

	if (cursor_pos > 2000)
	{
		for (int i=0; i<4000; i+=2)
			*((word *)(0x000B8000 + i)) =
				*((word *)(0x000B80A0 + i));
		cursor_pos -= 0x50;
	}

	// set cursor position
	outb(0x0E, 0x03D4);
	outb(cursor_pos>>8, 0x03D5);
	outb(0x0F, 0x03D4);
	outb(cursor_pos, 0x03D5);
}

void puts(char *str)
{
	while (*str)
	{
		putchar(*str);
		str++;
	}
}

// Hardware main loader
void loader_hw(void)
{
	// Get cursor position from vga registers.
	outb(0x0E, 0x03D4);
	cursor_pos = inb(0x03D5) << 8;
	outb(0x0F, 0x03D4);
	cursor_pos |= inb(0x03D5);

	// Tell the (l)user what we are up to.
	puts("OK\nInitializing hardware...");

	// Initialise the PIC.
//	outb(0x11, 0x20);
//	outb(0x20, 0x21);
//	outb(0x04, 0x21);
//	outb(0x01, 0x21);
//	outb(0xFF, 0x21);
}

