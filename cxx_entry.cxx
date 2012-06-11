//      cxx_entry.cxx
//
//      Copyright 2011 D.L.Buunk <dlbuunk@gmail.com>
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

//	This is the C++ entrypoint of the kernel, it is responsible for
//	ordering the heap to be initialized and calling the main startup
//	sequence, system::init().

#include "util.hxx"
using util::kputs;
using util::kprintf;

#include "system.hxx"
#include "memory.hxx"

// These are the global overloads of new and delete.

void * operator new(size_t size)
{
	return memory::heapalloc->malloc(size);
}

void * operator new[](size_t size)
{
	return memory::heapalloc->malloc(size);
}

void operator delete(void * p)
{
	return memory::heapalloc->free(p);
}

void operator delete[](void * p)
{
	return memory::heapalloc->free(p);
}

// And a few overloads needed for placement new.
void * operator new(size_t, void * p) { return p; }
void * operator new[](size_t, void * p) { return p; }
void operator delete(void *, void *) { }
void operator delete[](void *, void *) { }

// Special array, needed to hold the main HeapAlloc object.
byte heap_alloc_array[sizeof(memory::HeapAlloc)];

// Main function, called from __c_entry().
extern "C" void __cxx_entry(memory::LoaderData * loaderdata)
{
	// This should be before any use of util::kputs() or deriviatives!
	util::_loader_puts = loaderdata->puts;

	// If we are running an old 386, bail out.
	// On a 386, both 'lock cmpxchgb' and 'inldpg' are unavailable.
	if (*((byte *) 0x2DFE) == 3)
	{
		kputs("cxx_entry: Error, ViOS cannot run on an old 386.");
		return;
	}

	// Init the paging code.
	kputs("cxx_entry: Initializing the main paging code.");
	memory::page_init(loaderdata);

	// Init main heap.
	kputs("cxx_entry: Trying to create the main heap.");
	memory::heapalloc = new ((void *) heap_alloc_array) memory::HeapAlloc();
	size_t f = memory::heapalloc->status();
	if (f == 0)
	{
		kputs("cxx_entry: And we failed, exiting.");
		return;
	}
	f >>= 10;
	f++;
	kprintf("%t cxx_entry: And we have %u KiB allocated in the heap.\n", f);

	// And run the main startup routine in system.
	system::init_nt(loaderdata->mem_low);

	memory::heapalloc->~HeapAlloc();
}
