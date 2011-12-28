//      loader_m.c
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

// Hardware functions
extern void puts(char *);
extern void timer(int ticks);
extern void read_block(void * addr, word block);
extern void exit_hw(void);

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

// Loader main function
void loader_main(void)
{
	// Tell the (l)user what we are up to.
	puts("Kernel  ");
	read_block((void *) 0xC000, *((word *) 0x1210));
	read_block((void *) 0xD000, *((word *) 0x1218));

	word block;
	for (int i=0; i<8; i++)
		if(! strcmp((char *)((i<<5)+0xD008), "kernel.bin"))
		{
			read_block((void *) 0xE000,
				block = *((word *)((i<<5)+0xD000)));

		//	block = *((word *)((block<<1)+0xC000));
		//	if (block < 0xFFF0)
		//		read_block((void *) 0xF000, block);

			// now, run the kernel
			(**((void (**)(
				void (*)(void *, word),
				void (*)(char *),
				void (*)(int),
				void (*)(void)
			)) (0xE000 + *((word *) 0xE000))))(
				&read_block,
				&puts,
				&timer,
				&exit_hw
			);
			return;
		}
	puts("FAIL");
}
