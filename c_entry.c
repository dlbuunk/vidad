//      c_entry.c
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

//	This is the C entry point of the kernel, it is used for setting up
//	some specifics before running any C++ code. Those specifics include
//	things like ctors && dtors, and clearing the bss.

// General typedefs.
typedef unsigned char byte;
typedef unsigned short int word;
typedef unsigned int dword;
typedef unsigned long long int qword;

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

// Externals: bss, ctor, and dtor are here.
extern dword bss_start;
extern dword bss_end;
extern void ctor_start(void);
extern void ctor_end(void);
extern void dtor_start(void);
extern void dtor_end(void);

extern void __cxx_entry(struct LoaderData *);

// Main function, called from __kernel_entry.
void __c_entry(struct LoaderData * loaderdata)
{
	// Clear the BSS.
	for (dword * i = &bss_start; i < &bss_end; i++)
		*i = 0;

	// Tell the (l)user what we are up to.
	(*loaderdata->puts)("[    :  :  .  ] Kernel running, .bss cleared.\n");

	// Call the constructors.
	for (void (*i)(void) = &ctor_start; i < &ctor_end;)
		(*i++)();

	// Call the C++ entry point.
	__cxx_entry(loaderdata);

	// Call the destructors.
	for (void (*i)(void) = &dtor_end; i > &dtor_start;)
		(*--i)();
	(*loaderdata->puts)("[    :  :  .  ] Kernel shutting down.\n");
}
