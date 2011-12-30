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



// Memory info structs
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



// Helper functions
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

void printw(word val)
{
	char out[5];
	for (int i=0; i<4; i++)
	{
		out[i] = ((val >> ((3-i)<<2)) & 0xF) + 0x30;
		if (out[i] > 0x39)
			out[i] += 7;
	}
	out[4] = '\0';
	(*_puts)(out);
}

void printd(dword val)
{
	char out[9];
	for (int i=0; i<8; i++)
	{
		out[i] = ((val >> ((7-i)<<2)) & 0xF) + 0x30;
		if (out[i] > 0x39)
			out[i] += 7;
	}
	out[8] = '\0';
	(*_puts)(out);
}



// Paging init main functions

byte * mem_map;
dword ** stack_pages;
dword * page_stack;

void * page_alloc(dword lin)
{
	lin &= 0xFFFFF000;
	if (lin >= 0x01000000)
		return 0;
	dword phys = *--page_stack << 12;
	if (! ((dword)page_stack | 0xFFF))
		page_stack = *--stack_pages;
	mem_map[lin>>12] = phys | 0x03;
	return (void *) phys;
}

void page_init(
	int (*read_file)(void *, char *, word),
	void (*puts)(char *),
	void (*timer)(int),
	void (*exit_hw)(void))
{
	// Make puts usefull for other functions.
	_puts = puts;

	// Load the second part of the paging init code.
	if (*((byte *) 0xE02B) == 2)
	{
		if ((*read_file)((void *) 0xF000, "kernel.bin", 1))
		{
			(*puts)("[] Error: cannot load second block"
				" of kernel.\n");
				return;
		}
	}
	else if (*((byte *) 0xE02B) > 2)
	{
		(*puts)("[] Error, paging initialization code too large\n");
		return;
	}
	else if (*((byte *) 0xE02B) ==  0)
		(*puts)("[] Warning, kernel header possibly corrupted.\n");

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
	// 1 is useable, 0 is unuseable.
	mem_map = (byte *) 0x10000;
	dword mstart, mend;
	for (int i=0; i<4096; i++)
		mem_map[i] = 0;
	if (info->num_entries)
	{
		for (int i=0; i<info->num_entries; i++)
			if (meminfo[i].type == 1)
			{
				mstart = ((meminfo[i].start-1) >> 12) + 1;
				mend = (meminfo[i].start+meminfo[i].size) >> 12;
				if (mstart > 0x1000)
					mstart = 0x1000;
				if (mend > 0x1000)
					mend = 0x1000;
				for (dword j=mstart; j<mend; j++)
					mem_map[j] = 1;
			}
		for (int i=0; i<info->num_entries; i++)
			if (meminfo[i].type != 1)
			{
				mstart = ((meminfo[i].start-1) >> 12) + 1;
				mend = (meminfo[i].start+meminfo[i].size) >> 12;
				if (mstart > 0x1000)
					mstart = 0x1000;
				if (mend > 0x1000)
					mend = 0x1000;
				for (dword j=mstart; j<mend; j++)
					mem_map[j] = 0;
			}
	}
	else if (info->mem_mid)
	{
		mstart = 0x100;
		mend = info->mem_mid >> 2;
		for (dword j=mstart; j<mend; j++)
			mem_map[j] = 1;
		for (dword j=0; j<mem_low; j++)
			mem_map[j] = 1;
	}
	else if (info-> mem_alt)
	{
		mstart = 0x100;
		mend = info->mem_alt >> 2;
		if (mend > 0x1000)
			mend = 0x1000;
		for (dword j=mstart; j<mend; j++)
			mem_map[j] = 1;
		for (dword j=0; j<mem_low; j++)
			mem_map[j] = 1;
	}
	else // Assume 14 MiB high memory.
	{
		for (dword j=0; j<mem_low; j++)
			mem_map[j] = 1;
		(*puts)("[] Warning: BIOS reports no memory above 1 MiB"
			", assuming 14 MiB present");
	}



	// Thirdly, setup the free page stack.
	stack_pages = (dword **) 0x00100000;
	for (int k=0; k<0x1000; k++)
		stack_pages[k] = (dword *) 0;
	page_stack = (dword *) 0;
	int i = 0x101;
	while (i<0x1000 && ! mem_map[i]) i++;
	if (i == 0x1000)
	{
		(*puts)("[] Error: cannot find free page for page-stack");
		return;
	}

	for (int k=0; k<4; k++)
	{
		while (i<0x1000 && ! mem_map[i]) i++;
		if (i == 0x1000)
			goto end_stack;
		page_stack = (dword *) (i<<12);
		*stack_pages++ = page_stack;
		*page_stack++ = i++;
		for (int j=0; j<0x3FF; j++)
		{
			while (i<0x1000 && ! mem_map[i]) i++;
			if (i == 0x1000)
				goto end_stack;
			*page_stack++ = i++;
		}
	}

	end_stack: ;



	// Fourthly, setup the page tables.
	dword * page_table = (dword *) 0x4000;
	// ID-page the first 1 MB, no cache.
	for (int i=0; i<0x100; i++)
		page_table[i] = (i<<12) | 0x1B;
	// Clear the rest of the page tables.
	for (int i=0x100; i<0x1000; i++)
		page_table[i] = 0;



	// Fiftly, load the kernel.
	byte kernel_start = *((byte *) 0xE02B);
	dword kernel_size = *((dword *) 0xE02C);
	int need_load = 1;
	for (dword j=0; j<kernel_size; j++)
	{
		void * phys = page_alloc((j<<12)+0x00100000);
		if (! phys)
		{
			(*puts)("[] Error: out of memory\n");
			return;
		}
		if (need_load)
		{
			int r=(*read_file)(phys, "kernel.bin", j+kernel_start);
			if (r == 1) need_load = 0;
			else if (r == -1)
			{
				(*puts)("[] Error, kernel not found.\n");
				return;
			}
		}
	}



	// Void-cast all unused arguments to please -Wall -Werror.
	(void) timer;
	(void) exit_hw;
}
