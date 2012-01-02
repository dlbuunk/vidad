//      malloc.cxx
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

HeapAlloc * heapalloc;

HeapAlloc::HeapAlloc()
{
	first = (Mobject *) page_alloc(32); // 128 KiB should be enough...
	first->pages = 32;
	first->size = 32<<12;
	first->used = 0;
	first->next = 0;
	first->prev = 0;
}

HeapAlloc::~HeapAlloc()
{
	// Okay, deallocate all memory...
	void * ptr = 0;
	long int num = 0;
	// Calling page_free() with num==0 does not blow thing up.
	Mobject * obj = first;
	while (obj->next)
		if (obj->pages)
		{
			// We have reached the next page range,
			// deallocate the previous one.
			page_free(ptr, num);
			// And mark this one for later deallocation.
			ptr = (void *) obj;
			num = obj->pages;
		}
	// And free the last remaining memory.
	page_free(ptr, num);
}

void * HeapAlloc::malloc(size_t size)
{
	(void) size;
	return 0;
}

void HeapAlloc::free(void * ptr)
{
	(void) ptr;
}

}
