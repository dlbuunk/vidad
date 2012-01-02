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

#define PAGE_NUM 32

namespace memory
{

HeapAlloc * heapalloc;

HeapAlloc::HeapAlloc()
{
	first = (Mobject *) page_alloc(PAGE_NUM); // 128 KiB should be enough...
	if (! first)
	{
		kputs("memory::HeapAlloc: Failed to init the requested heap.");
		return;
		// There is no other way to communicate this message,
		// so everyone is expected to see if status() returns nonzero
		// as it should. (And abort otherwise)
	}
	first->pages = PAGE_NUM;
	first->size = (PAGE_NUM<<12) - sizeof(Mobject);
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
	// Find a large enough object...
	Mobject * obj = first;
	while (1)
	{
		while (obj->used && obj->next)
			obj = obj->next;
		if (! obj->next)
		{
			// We are at the end of the chain.
			if (obj->size >= size)
			{
				// Okay, there is enough memory, allocate.
				obj->next = (Mobject *)
					((size_t) obj + size + sizeof(Mobject));
				obj->next->prev = obj;
				obj->next->next = 0;
				obj->next->size =obj->size-size-sizeof(Mobject);
				obj->next->pages = 0;
				obj->next->used = 0;
				obj->size = size;
				obj->used = 1;
				return (void *)((size_t) obj + sizeof(Mobject));
			}
			// And there is not enough memory, page more in.
			long int num_pages = PAGE_NUM;
			if (((size-1+sizeof(Mobject))>>12) + 1 > PAGE_NUM)
				num_pages = ((size-1+sizeof(Mobject))>>12) + 1;
			if (! (obj->next = (Mobject *) page_alloc(num_pages)))
			{
				// Paging failed, retry.
				num_pages = ((size-1+sizeof(Mobject))>>12) + 1;
				if(!(obj->next=(Mobject*)page_alloc(num_pages)))
					return 0;
			}
			// New memory is paged in.
			obj->next->prev = 0;
			obj = obj->next;
			obj->next = 0;
			obj->pages = num_pages;
			obj->used = 1;
			
		}
	}
}

void HeapAlloc::free(void * ptr)
{
	(void) ptr;
}

}
