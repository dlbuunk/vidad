//      page_init.c
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
typedef unsigned long long int qword;

// memory info structs
struct MemInfo
{
	qword start;
	qword size;
	dword type;
};

struct Info
{
	word mem_high;
	word mem_mid;
	word mem_alt;
	word mem_low;
	word hw_info;
	byte cpu_type; // == 3
	byte num_entries;
};

// helper function
void (*_puts)(char *);

void printb(byte val)
{
	char out[3];
	out[0] = (val>>4) + 0x30;
	if (out[0] > 0x39)
		out[0] += 7;
	out[1] = (val&0x0F) + 0x30;
	if (out[1] > 0x39)
		out[1] += 7;
	out[2] = '\0';
	(*_puts)(out);
}

// Paging init main function
void page_init(
	int (*read_file)(void *, char *, word),
	void (*puts)(char *),
	void (*timer)(int),
	void (*exit_hw)(void))
{
	// Make puts usefull for other functions.
	_puts = puts;

	// Load the second part of the paging init code.
	if ((*read_file)((void *) 0xF000, "kernel.bin", 1))
		(*puts)("[] Warning: cannot load second block of kernel.\n");

	// Define the meminfo structs.
	struct Info * info = (struct Info *) 0x2DF4;
	struct MemInfo * meminfo = (struct MemInfo *) 0x2C00;

	// Firstly, figure out how much low memory we have.
	unsigned int mem_low = 0; // blocks
	for (int i=0; i<info->num_entries; i++)
	{
		if (meminfo[i].start < 0x1000 && meminfo[i].type == 1)
			mem_low = (meminfo[i].start + meminfo[i].size) >> 12;
	}
	// By conflicting, but both valid values, the lowest takes precedence.
	if (info->mem_low>>2 < mem_low && info->mem_low >= 128)
		mem_low = info->mem_low >> 2;
	// If the advanced method does not return any data, trust int 0x12.
	if (! mem_low)
		mem_low = info->mem_low >> 2;
	// If we still have a value that is way too low, use the default.
	if (mem_low < 32) // 128 KiB
	{
		(*puts)("[] Warning: BIOS reports less than 128 KiB of low "
			"memory, system may crash.\n");
		mem_low = 128; // 512 KiB
	}
	// If there is less than 128 KiB low memory, we are in trouble
	// anyway, so it is better to assume that the BIOS is wrong.

	// Secondly, create a memory map for the first 16 MiB

	// Void-cast all unused arguments to please -Wall -Werror.
	(void) timer;
	(void) exit_hw;
}
