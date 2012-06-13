//      loader.c
//
//      Copyright 2011,2012 D.L.Buunk <dlbuunk@gmail.com>
//
//      This file is part of ViOS.
//
//      ViOS is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//
//      ViOS is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//
//      You should have received a copy of the GNU General Public License
//      along with ViOS; if not, write to the Free Software
//      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
//      MA 02110-1301, USA.

// General typedefs
typedef unsigned char byte;
typedef unsigned short int word;
typedef unsigned int dword;

// Printing function
extern void puts(char *);

// Helper function
int strcmp(char * s1, char * s2)
{
	while (*s1 == *s2)
	{
		if (! *s1) return 0;
		s1++;
		s2++;
	}
	return -1;
}

// Exported functions
int read_file(byte * addr, char * filename, word sect_no)
{
	if (strcmp(filename, "KERNEL.BIN"))
		return -1;
	byte * src = (byte *) (0x00020000 + ((dword)sect_no << 9));
	for (int i = 0; i < 512; i++)
		*addr++ = *src++;
	return 0;
}

void exit_hw(void)
{
	// do-nothing stub
}

// Loader main function
void loader(void)
{
	// Tell the (l)user what we are up to.
	puts("OK\nKernel  ");

	// 'Load' the kernel
	for (int i=0; i<16; i++)
	{
		if (read_file((byte *) (0xE000 + (i<<9)), "KERNEL.BIN", i))
		{
			puts("FAIL");
			return;
		}
	}

	puts("OK\n");

	// now, run the kernel
	(**((void (**)(
		int (*)(byte *, char *, word),
		void (*)(char *),
		void (*)(void)
	)) 0xE000))(
		&read_file,
		&puts,
		&exit_hw
	);
}
