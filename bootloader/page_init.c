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



// Memory info structs & loader struct
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

struct
{
	int (*read_file)(void *, char *, word);
	void (*puts)(char *);
	void (*timer)(int);
	void (*exit_hw)(void);
	dword ** stack_pages;
	dword * page_stack;
	dword mem_low;
} loaderdata;



// Helper functions
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
	(*loaderdata.puts)(out);
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
	(*loaderdata.puts)(out);
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
	(*loaderdata.puts)(out);
}



// Paging init main functions

byte * mem_map;
dword * page_table;
dword ** stack_pages;
dword * page_stack;

void * page_alloc(dword lin)
{
	lin &= 0xFFFFF000;
	if (lin >= 0x01000000)
		return 0;
	dword phys = *--page_stack << 12;
	*page_stack = 0;
	if (! ((dword)page_stack | 0xFFF))
		page_stack = *--stack_pages;
	page_table[lin>>12] = phys | 0x07;
	return (void *) phys;
}

extern dword bss_start;
extern dword bss_end;

void page_init(
	int (*read_file)(void *, char *, word),
	void (*puts)(char *),
	void (*timer)(int),
	void (*exit_hw)(void))
{
	// Clear the BSS.
	for (dword * i = &bss_start; i < &bss_end; i++)
		*i = 0;

	// Tell the (l)user what we are up to.
	(*puts)("[] Start of page_init().\n");

	// Init the loaderdata.
	loaderdata.read_file = read_file;
	loaderdata.puts = puts;
	loaderdata.timer = timer;
	loaderdata.exit_hw = exit_hw;

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
	(*puts)("[] Calculating amount of low memory.\n");
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



	// Secondly, create a memory map for the first 16 MiB,
	// 1 is useable, 0 is unuseable.
	(*puts)("[] Building memory map.\n");
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
			", assuming 14 MiB present\n");
	}



	// Thirdly, setup the free page stack.
	(*puts)("[] Setting up free page stack.\n");
	stack_pages = (dword **) 0x00100000;
	for (int k=0; k<0x1000; k++)
		stack_pages[k] = (dword *) 0;
	page_stack = (dword *) 0;
	int i = 0x101;
	while (i<0x1000 && ! mem_map[i]) i++;
	if (i == 0x1000)
	{
		(*puts)("[] Error: cannot find free page for page-stack\n");
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
	(*puts)("[] Setup page tables.\n");
	page_table = (dword *) 0x4000;
	// ID-page the first 1 MB, no cache.
	for (int i=0; i<0x100; i++)
		page_table[i] = (i<<12) | 0x1F;
	// Clear the rest of the page tables.
	for (int i=0x100; i<0x1000; i++)
		page_table[i] = 0;



	// Fiftly, load the kernel.
	(*puts)("[] Loading kernel.\n");
	byte kernel_start = *((byte *) 0xE02B);
	dword kernel_size = *((dword *) 0xE02C);
	int need_load = 1;
	void * phys;
	for (dword j=0; j<kernel_size; j++)
	{
		phys = page_alloc((j<<12)+0x00100000);
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



	// Sixthly, if on an 486+, temporary disable the cache.
	(*puts)("[] Temporary disable cache.\n");
	int is_486;
	asm volatile (
		"pushfl\n\t"
		"popl	%%eax\n\t"
		"movl	%%eax,%%ecx\n\t"
		"xorl	$0x00040000,%%eax\n\t"
		"pushl	%%eax\n\t"
		"popfl\n\t"
		"pushfl\n\t"
		"popl	%%eax\n\t"
		"xorl	%%ecx,%%eax\n\t"
		"pushl	%%ecx\n\t"
		"popfl\n\t"
		: "=a" (is_486)
		:
		: "%ecx"
		) ;
	if (is_486)
		asm volatile (
			"movl	%%cr0,%%eax\n\t"
			"andl	$0xDFFFFFFF,%%eax\n\t" // write-trough caching
			"orl	$0x40000000,%%eax\n\t" // no cache
			"wbinvd\n\t"
			"movl	%%eax,%%cr0\n\t"
			"invd\n\t"
			:
			:
			: "%eax"
			) ;



	// Seventhly, move the GDT and setup the (empty) sysLDT.
	(*puts)("[] Re-init GDT.\n");
	for (dword * j = (dword *) 0x00010000; j < (dword *) 0x00020000; j++)
		*j = 0;
	for (dword * j = (dword *) 0x00002000; j < (dword *) 0x00002C00; j++)
		*j = 0;
	// sys-LDT
	*((qword *) 0x10008) = 0x0000820020000C00LL;
	// 16-bit
	*((qword *) 0x10010) = 0x00009A000000FFFFLL;
	*((qword *) 0x10018) = 0x000092000000FFFFLL;
	// 32-bit
	*((qword *) 0x10020) = 0x00CF9A000000FFFFLL;
	*((qword *) 0x10028) = 0x00CF92000000FFFFLL;
	// GDTP
	*((word *) 0x2E02) = 0xFFFF;
	*((dword *) 0x2E04) = 0x00010000;
	// Install, and reload segment registers.
	asm volatile (
		"lgdt	0x2E02\n\t"
		"jmpl	$0x0020,$__special_GDT_jump\n"
		"__special_GDT_jump:\n\t"
		"movw	$0x0028,%%ax\n\t"
		"movw	%%ax,%%ss\n\t"
		"movw	%%ax,%%ds\n\t"
		"movw	%%ax,%%es\n\t"
		"movw	%%ax,%%fs\n\t"
		"movw	%%ax,%%gs\n\t"
		"movw	$0x0008,%%ax\n\t"
		"lldt	%%ax\n\t"
		:
		:
		: "memory"
		) ;



	// Eightly, create page directory, load CR3 and enable paging and cache.
	(*puts)("[] Start paging.\n");
	for (dword * j = (dword *) 0x3000; j < (dword *) 0x4000; j++)
		*j = 0;
	*((dword *) 0x3000) = 0x00004007;
	*((dword *) 0x3004) = 0x00005007;
	*((dword *) 0x3008) = 0x00006007;
	*((dword *) 0x300C) = 0x00007007;
	asm volatile (
		"movl	$0x00003000,%%eax\n\t"
		"movl	%%eax,%%cr3\n\t"
		"movl	%%cr0,%%eax\n\t"
		"orl	$0x80000000,%%eax\n\t"
		"movl	%%eax,%%cr0\n\t"
		:
		:
		: "%eax"
		, "memory"
		) ;
	if (is_486)
		asm volatile (
			"movl	%%cr0,%%eax\n\t"
			"andl	$0xBFFFFFFF,%%eax\n\t"
			"movl	%%eax,%%cr0\n\t"
			:
			:
			: "%eax"
			) ;



	// Ninthly, map the page stack into kernel memory,
	// and switch to logical addresses in stack_pages.
	(*puts)("[] Switching to linear addresses.\n");
	*((dword *)((kernel_size<<2) + 0x4400)) = 0x00100007;
	stack_pages += kernel_size << 10; // stack_pages is a pointer!
	for (int j=0; j<4; j++)
	{
		*((dword *)((kernel_size<<2) + 0x4404 + (j<<2))) =
			((dword *)((dword)stack_pages & 0xFFFFF000))[j] | 7;
		((dword *)((dword)stack_pages & 0xFFFFF000))[j] =
			(kernel_size + 0x101 + j) << 12;
	}
	dword temp = (dword) page_stack;
	temp &= 0x00000FFF;
	temp += (dword) *--stack_pages;
	stack_pages++;
	page_stack = (dword *) temp;



	// Make some final preparations and call the kernel.
	(*puts)("[] Calling kernel proper.\n");
	loaderdata.stack_pages = stack_pages;
	loaderdata.page_stack = page_stack;
	loaderdata.mem_low = mem_low;
	if (is_486)
		info->cpu_type = 4;
	(*((void (*)(void *)) 0x00100000))((void *) &loaderdata);
}
