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
using util::kprintf;

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
	void * laddr;
	dword paddr;
	long int lpage;
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
				lpage = (i<<10) + j;
				laddr = (void *)((i<<22) + (j<<12));
				goto phys_alloc;
			}
		}
	}
	// Non enough linear adrress space, return null pointer.
	kputs("memory::page_alloc: ERROR: out of linear adrress space.");
	return (void *) 0;

	// Now allocate physical pages
	phys_alloc:;
	for (int i=lpage>>10; i<=(lpage+num)>>10; i++)
	{
		for(int j=((i<(lpage+num)>>10)?lpage&0x3FF:0);
			j<((i<(lpage+num)>>10)?0x400:(lpage+num)&0x3FF); j++)
		{
			kprintf("%t memory::page_alloc: %u %u\n", i, j);
			paddr = *--page_stack;
			*page_stack = 0;
			if (! ((dword) page_stack & 0xFFF))
			{
				page_stack = *--stack_pages;
				*stack_pages = 0;
				if (! ((dword) stack_pages & 0xFFF))
				{
					// And we are OOM.
					kputs("memory::page_alloc: "
					"ERROR: out of memory.");
					return (void *) 0;
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
		}
	}
	(void) paddr;
	return laddr;
}

}
