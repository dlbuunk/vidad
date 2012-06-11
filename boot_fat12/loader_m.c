//      loader_m.c
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

// Hardware functions
extern void puts(char *);
extern void timer(int ticks);
extern void read_sect(byte * addr, word sect);
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

// FAT12 struct and other data
#define FAT_ADDR 0xC800		// 6 kb under the limit, should be enough

struct dir_entry
{
	char name[8];
	char ext[3];
	byte attrib;
	byte resvd;
	byte create_time_ss;
	word create_time;
	word create_date;
	word acc_date;
	word high_sect_num; // == 0
	word mod_time;
	word mod_date;
	word low_sect_num;
	dword size;
};

word num_resvd_sectors;
word num_fat_sectors;
byte num_fats;
word num_root_dir_sectors;
word * FAT;
struct dir_entry * root_dir;

// Function to load a sector within a file, given a first sector.
int read_file_num(byte * addr, word sect, word sect_count)
{
	if (sect_count) for (word i = 0; i< sect_count; i++)
	{
		if (sect & 0x0001)
			sect = FAT[sect + (sect>>1)] >> 4;
		else
			sect = FAT[sect + (sect>>1)] & 0x0FFF;
		if (sect >= 0xFF8)
			return 2;
	}

	read_sect(addr, sect);

	return 0;
}

// Function to load a sector within a file, given a name.
int read_file(byte * addr, char * filename, word sect_no)
{
	// Turn name into 8.3 format.
	char name[8];
	char ext[3];
	for (int i = 0; i < 8; i++) name[i] = ' ';
	for (int i = 0; i < 3; i++) ext[i] = ' ';
	for (int i = 0; i < 8; i++)
	{
		if (*filename == '.' || *filename == '\0')
			break;
		name[i] = *filename++;
	}

	if (*filename++) for (int i = 0; i < 3; i++)
	{
		if (! *filename)
			break;
		ext[i] = *filename++;
	}

	for (int i = 0; i < num_root_dir_sectors<<4; i++)
	{
		for (int j = 0; j < 8; j++)
			if (name[j] != root_dir[i].name[j]) goto next;
		for (int j = 0; j < 3; j++)
			if (ext[j] != root_dir[i].ext[j]) goto next;
		// FOUND, now load it.
		return read_file_num(addr, root_dir[i].low_sect_num, sect_no);
		next: ;
	}

	// Not found.
	return 1;
}

// Loader main function
void loader_main(void)
{
	// Tell the (l)user what we are up to.
	puts("Kernel  ");

	// load FAT and root directory
	num_resvd_sectors = *((word *) 0x60E);
	num_fat_sectors = *((word *) 0x0616);
	num_fats = *((byte *) 0x0610);
	num_root_dir_sectors = *((word *) 0x0611) >> 4;
	FAT = (word *) FAT_ADDR;
	root_dir = (struct dir_entry *)(FAT_ADDR + (num_fat_sectors << 9));

	for (int i = 0; i < num_fat_sectors; i++)
		read_sect((byte *) (FAT_ADDR + (i<<9)), num_resvd_sectors + i);

	for (int i = 0; i < num_root_dir_sectors; i++)
		read_sect((byte *) (FAT_ADDR + ((num_fat_sectors+i)<<9)),
			num_resvd_sectors + num_fat_sectors*num_fats + i);

	// read the kernel
	for (int i=0; i<16; i++)
	{
		if (read_file((byte *) 0xE000, "kernel.bin", 0))
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
		void (*)(int),
		void (*)(void)
	)) (0xE000 + *((word *) 0xE000))))(
		&read_file,
		&puts,
		&timer,
		&exit_hw
	);
}
