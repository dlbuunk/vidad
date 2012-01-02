//      page.cxx
//      
//      Copyright 2012 D.L.Buunk <dlbuunk@gmail.com>
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

#include "util.hxx"
using util::kputs;

#include "memory.hxx"

namespace memory
{

dword ** stack_pages;
dword * page_stack;
Pentry * PD;

void page_init(LoaderData * loaderdata)
{
	// Look, this is very simple, all paging structures are already set up,
	// so the only thing we have to do is initialise a few pointers to them.
	stack_pages = loaderdata->stack_pages;
	page_stack = loaderdata->page_stack;
	PD = (Pentry *) 0x3000;
}

void * page_alloc(long int num)
{
	// Firstly, find a location in linaddr where we can put the
	// requested number of pages
	void * laddr = get_laddr(num);
	long int lpage = (long int) laddr >> 12;

	if (! lpage)
	{
		// Non enough linear adrress space, return null pointer.
		kputs("memory::page_alloc: ERROR: out of linear adrress space."
			);
		return (void *) 0;
	}

	// Now allocate physical pages
	int init_i = lpage>>10;
	for (int i=lpage>>10; i<=(lpage+num)>>10; i++)
		for (int j=((i==init_i)?lpage&0x3FF:0);
			j<((i<(lpage+num)>>10)?0x400:(lpage+num)&0x3FF); j++)
			if (phys_alloc(i, j))
				return 0;
	return laddr;
}

void * page_realloc(void * ptr, long int nold, long int nnew)
{
	ptr = (void *)((dword)ptr & 0xFFFFF000);

	if (nold == nnew)
		return ptr;
	if (nold > nnew)
	{
		page_free((void *)((dword)ptr + (nnew<<12)), nold-nnew);
		return ptr;
	}
	// Okay, we have to increase the size of the memory range.
	// First see if we can do that without moving.
	int i = (((dword)ptr>>12) + nold) >> 10;
	int imax = (((dword)ptr>>12) + nnew) >> 10;
	int iorig = i;
	int j = (((dword)ptr>>12) + nold) & 0x3FF;
	int jmax = (((dword)ptr>>12) + nnew) & 0x3FF;
	for (; i<=imax; i++)
		for (j = (i==iorig ? j : 0); j < (i<imax ? 0x400 : jmax); j++)
			if (PD[i][j].present) goto fail;
	// We succeeded!
	// Map in the new pages.
	i = (((dword)ptr>>12) + nold) >> 10;
	j = (((dword)ptr>>12) + nold) & 0x3FF;
	for (; i<=imax; i++)
		for (j = (i==iorig ? j : 0); j < (i<imax ? 0x400 : jmax); j++)
			if (phys_alloc(i, j))
				return 0;
	return ptr;

	fail:; // We could not extend the memory range, find a new one.
	void * laddr = get_laddr(nnew);
	if (! laddr)
		return 0;
	// Okay, found, map in the original part.
	// Note that we do not touch the physical pages.
	// Here is i, j for target, p, q for source.
	int p = (dword)ptr >> 22;
	int q = ((dword)ptr >> 12) & 0x3FF;
	i = (dword)laddr >> 22;
	imax = (((dword)laddr>>12) + nold) >> 10;
	iorig = i;
	j = ((dword)laddr >> 12) & 0x3FF;
	jmax = (((dword)laddr>>12) + nold) & 0x3FF;
	for (; i<=imax; i++, p++)
		for (j = (i==iorig ? j:0); j < (i<imax ? 0x400:jmax); j++, q++)
		{
			PD[i][j] = PD[p][q];
			PD[p][q].present = 0;
			PD[p][q].ptr = 0;
			clear_tlb((void *)((p<<22) + (q<<12)));
		}

	// Finally, map in the new memory.
	if (j & 0x3FF)
		i--;
	else
		j = 0;
	iorig = i;
	imax = (((dword)laddr>>12) + nnew) >> 10;
	jmax = (((dword)laddr>>12) + nnew) & 0x3FF;
	for (; i<=imax; i++)
		for (j = (i==iorig ? j : 0); j < (i<imax ? 0x400 : jmax); j++)
			if (phys_alloc(i, j))
				return 0;
	return laddr;
}

void page_free(void * ptr)
{
	// Cut off the pointer.
	ptr = (void *)((dword) ptr & 0xFFFFF000);

	// Calculate the indices.
	int i = (dword)ptr >> 22;
	int j = (dword)ptr >> 12;

	if (! PD[i][j].present)
	{
		kputs("page_free: cannot unmap a page that is not mapped");
		return;
	}

	// If the page-stack page is full, keep the page mapped.
	if (! ((dword) page_stack & 0xFFF))
	{
		*stack_pages++ = page_stack = (dword *) ptr;
		PD[i][j].present = 1;
		PD[i][j].writea = 1;
		PD[i][j].user = 1;
		PD[i][j].wr_thr = 0;
		PD[i][j].cache = 0;
		PD[i][j].access = 0;
		PD[i][j].dirty = 0;
		PD[i][j].size = 0;
		PD[i][j].global = 0;
		PD[i][j].avail = 0;
		clear_tlb(ptr);
		*page_stack++ = PD[i][j].ptr;
		return;
	}

	// Wipe the entry from the page table.
	dword paddr = PD[i][j].ptr;
	PD[i][j].present = 0;
	PD[i][j].ptr = 0;
	// Place the entry on the page stack.
	*page_stack++ = paddr;
	clear_tlb(ptr);
}

void page_free(void * ptr, long int num)
{
	for (long int i=0; i<num; i++)
	{
		page_free(ptr);
		ptr = (byte *)ptr + 0x1000;
	}
}

int phys_alloc(int i, int j)
{
	dword paddr = *--page_stack;

	// If the page-stack page is empty...
	if (! ((dword) page_stack & 0xFFF))
	{
		// ...map the page out.
		PD[(dword)*stack_pages>>22]
			[(dword)*stack_pages>>12].present = 0;
		PD[(dword)*stack_pages>>22]
			[(dword)*stack_pages>>12].ptr = 0;
		page_stack = *--stack_pages;

		if (! ((dword) stack_pages & 0xFFF))
		{
			// And we are OOM.
			kputs("memory::page_alloc: "
			"ERROR: out of memory.");
			return 1;
		}
	}
	PD[i][j].ptr = paddr;
	PD[i][j].present = 1;
	PD[i][j].writea = 1;
	PD[i][j].user = 1;
	PD[i][j].wr_thr = 0;
	PD[i][j].cache = 0;
	PD[i][j].access = 0;
	PD[i][j].dirty = 0;
	PD[i][j].size = 0;
	PD[i][j].global = 0;
	PD[i][j].avail = 0;
	return 0;
}

void * get_laddr(long int num)
{
	void * laddr;
	int k = num;
	for (int i=0; i<1024; i++)
	{
		if (! PD[i].present)
		{
			k = num;
			continue;
		}
		for (int j=0; j<1024; j++)
		{
			if (PD[i][j].present)
				k = num;
			else
				k--;
			if (! k) // address frame found.
			{
				j -= num - 1;
				while (j<0)
					i--, j+= 1024;
				laddr = (void *)((i<<22) + (j<<12));
				return laddr;
			}
		}
	}
	return 0;
}

void clear_tlb(void * addr)
{
	if (*((byte *)0x2DFE) >= 4)
		asm volatile ( "invlpg	%0\n\t" : : "m" (*(byte *)addr) : ) ;
	else
		asm volatile (
			"movl	%%cr3,%%eax\n\t"
			"movl	%%eax,%%cr3\n\t"
			:
			:
			: "%eax"
			) ;
}

}
