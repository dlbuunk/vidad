//      cxx_entry.cxx
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

//	This is the C++ entrypoint of the kernel, it is responsible for
//	ordering the heap to be initialized and calling the main startup
//	sequence, system::init().

#include "kernel.hxx"
#include "util/util.hxx"

// Loaderdata struct definition.
struct LoaderData
{
	void (*puts)(char const *);
	dword ** stack_pages;
	dword * page_stack;
	dword mem_low;
};

// Main function, called from __c_entry().
extern "C" void __cxx_entry(LoaderData * loaderdata)
{
	// This should be before any use of util::kputs() or deriviatives!
	util::_loader_puts = loaderdata->puts;

	// Do some testing.
	char buf1[80] = { "%t Testing util::memcpy()" } ;
	char buf2[80];
	util::memcpy(buf2, buf1, util::strlen(buf1));
	util::sprintf(buf1, "%s %s", buf2, "and util::sprintf().\n");
	util::memcpy(buf2, buf1, util::strlen(buf1));
	util::kprintf(buf2);

	//asm ( "int	$0x40\n\t" ) ;
}
