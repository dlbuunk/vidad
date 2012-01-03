//      malloc.cxx
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
	{
		if (obj->pages)
		{
			// We have reached the next page range,
			// deallocate the previous one.
			page_free(ptr, num);
			// And mark this one for later deallocation.
			ptr = (void *) obj;
			num = obj->pages;
		}
		obj = obj->next;
	}
	// And free the last remaining memory.
	page_free(ptr, num);
}

void * HeapAlloc::malloc(size_t size)
{
	// Align the size.
	size--;
	size &= ~(sizeof(size_t) - 1);
	size += sizeof(size_t);
	// Find a large enough object...
	Mobject * obj = first;
	while (1)
	{
		while (obj->used && obj->next)
			obj = obj->next;
		if (! obj->next)
		{
			// We are at the end of the chain.
			if (obj->size >= size && ! obj->used)
			{
				if (obj->size < sizeof(Mobject) + 16 + size)
				{
					// There is just enough memory.
					obj->used = 1;
					return (void *)(obj+1);
				}
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
				return (void *)(obj+1);
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
			obj->next->prev = obj;
			obj = obj->next;
			obj->next = 0;
			obj->pages = num_pages;
			obj->used = 1;
			obj->size = (num_pages << 12) - sizeof(Mobject);
			if (obj->size >= size + sizeof(Mobject) + 16)
			{
				// Split new block.
				obj->next = (Mobject *)
					((size_t) obj + size + sizeof(Mobject));
				obj->next->prev = obj;
				obj->next->next = 0;
				obj->next->size =obj->size-size-sizeof(Mobject);
				obj->next->pages = 0;
				obj->next->used = 0;
				obj->size = size;
			}
			return (void *)(obj+1);
		}

		// We aren't on the end of the chain.
		if (obj->size >= size)
		{
			if (obj->size < size + sizeof(Mobject) + 16)
			{
				// 'Perfect' fit, do not split.
				obj->used = 1;
				return (void *)(obj+1);
			}
			// Split the block.
			((Mobject *)((size_t)obj + sizeof(Mobject) + size))
				->next = obj->next;
			obj->next =(Mobject*)((size_t)obj+sizeof(Mobject)+size);
			obj->next->prev = obj;
			obj->next->pages = 0;
			obj->next->size = obj->size - size - sizeof(Mobject);
			obj->next->used = 0;
			obj->size = size;
			obj->used = 1;
			obj->next->next->prev = obj->next;
			return (void *)(obj+1);
		}
		// Nothing found, look further in the next cycle of the loop.
	}
}

void HeapAlloc::free(void * ptr)
{
	Mobject * obj = (Mobject *) ptr;
	obj--;
	obj->used = 0;

	// Merge forward blocks.
	while (obj->next && ! obj->next->used && ! obj->next->pages)
	{
		obj->size += sizeof(Mobject);
		obj->size += obj->next->size;
		obj->next->next->prev = obj;
		obj->next = obj->next->next;
	}

	// Merge backward blocks.
	while (obj->prev && ! obj->prev->used && ! obj->pages)
	{
		obj->prev->size += sizeof(Mobject);
		obj->prev->size += obj->size;
		obj->prev->next = obj->next;
		obj = obj->prev;
	}

	// See if we can unmap pages
	if (obj->size + sizeof(Mobject) == obj->pages << 12 && obj != first)
	{
		obj->prev->next = obj->next;
		obj->next->prev = obj->prev;
		page_free((void *) obj, obj->pages);
	}
}

size_t HeapAlloc::status()
{
	if (! first)
		return 0;

	Mobject * obj = first;
	size_t size = 0;
	do
	{
		if (! obj->used)
			size += obj->size;
		obj = obj->next;
	} while (obj);
	return size;
}

}
