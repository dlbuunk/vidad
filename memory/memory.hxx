//      memory.hxx
//      
//      Copyright 2012 D.L.Buunk <dlbuunk@gmail.com>
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
void clear_tlb(void * addr);
void * get_laddr(long int num);
int phys_alloc(int i, int j);

void * page_alloc(long int num);
void * page_realloc(void * ptr, long int nold, long int nnew);
void page_free(void * ptr);
void page_free(void * ptr, long int num);

// Heap management, malloc.cxx.
// As one instance of this class is created before new is functional
// (aka circular dependency) creating the first instance is quite, hmm, weird.
class HeapAlloc
{
	public:
	HeapAlloc();
	~HeapAlloc();
	void * malloc(size_t size);
	void free(void * ptr);
	size_t status();

	private:
	struct Mobject
	{
		Mobject * prev;
		Mobject * next;
		size_t size;
		unsigned long int pages;
		bool used;
	};
	Mobject * first;

}; // DO NOT create a global instance, for this would call page_alloc()
// before page_init() is run, thereby whrecking the page allocation system.

extern HeapAlloc * heapalloc;

}

#endif // MEMORY_HXX
