//	loader_print.c
//
//	Copyright 2011,2012 D.L.Buunk <dlbuunk@gmail.com>
//
//	This file is part of ViOS.
//
//	ViOS is free software; you can redistribute it and/or modify
//	it under the terms of the GNU General Public License as published by
//	the Free Software Foundation; either version 2 of the License, or
//	(at your option) any later version.
//
//	ViOS is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//	GNU General Public License for more details.
//
//	You should have received a copy of the GNU General Public License
//	along with ViOS; if not, write to the Free Software
//	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
//	MA 02110-1301, USA.

// General typedefs
typedef unsigned char byte;
typedef unsigned short int word;

// Common inb/outb functions
void outb(byte val, word port)
{
	asm volatile (
		"outb	%0,%1\n\t"
		:
		: "a" (val)
		, "Nd" (port)
		:
	);
}

byte inb(word port)
{
	byte val;
	asm volatile (
		"inb	%1,%0\n\t"
		: "=a" (val)
		: "Nd" (port)
		:
	);
	return val;
}

// Screen functions
void putchar(char ch)
{
	// Get cursor position from vga registers.
	word cursor_pos;
	outb(0x0E, 0x03D4);
	cursor_pos = inb(0x03D5) << 8;
	outb(0x0F, 0x03D4);
	cursor_pos |= inb(0x03D5);

	if (ch == '\n')
		cursor_pos = (cursor_pos/80 + 1) * 80;
	else if (ch == '\t')
		cursor_pos = ((cursor_pos>>3) + 1) << 3;
	else
		*((word *)(0x000B8000 + (cursor_pos++ <<1))) = 0x0700 | ch;

	if (cursor_pos >= 2000)
	{
		for (int i=0; i<4000; i+=2)
			*((word *)(0x000B8000 + i)) =
				*((word *)(0x000B80A0 + i));
		cursor_pos -= 0x50;
	}

	// Set cursor position.
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
