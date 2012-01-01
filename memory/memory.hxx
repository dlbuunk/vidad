//      memory.hxx
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

#ifndef MEMORY_HXX
#define MEMORY_HXX

#include "kernel.hxx"

namespace memory
{

// Loaderdata struct definition.
// BIG FAT NOTE: this struct appears in bootloader/page_init.c,
// c_entry.c and memory/memory.hxx.
// If changed in one place, update the others!
struct LoaderData
{
	void (*puts)(char const *);
	dword ** stack_pages;
	dword * page_stack;
	dword mem_low;
};

// Physical memory management, page.cxx.
struct Pentry
{
	unsigned present: 1;
	unsigned writea : 1;
	unsigned user   : 1;
	unsigned wr_thr : 1;
	unsigned cache  : 1;
	unsigned access : 1;
	unsigned dirty  : 1;
	unsigned size   : 1;
	unsigned global : 1;
	unsigned avail  : 3;
	unsigned ptr   : 20;

	Pentry& operator[](const int& i)
	{
		return ((Pentry *)(ptr << 12))[i];
	}
};

extern dword ** stack_pages;
extern dword * page_stack;
extern Pentry * PD;

void page_init(LoaderData * loaderdata);
void * page_alloc(long int num);

// Heap management, malloc.cxx.
void * malloc(size_t size);
void free(void * ptr);

}

#endif // MEMORY_HXX