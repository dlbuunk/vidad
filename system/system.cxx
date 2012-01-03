//      system.cxx
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

#include "system.hxx"

namespace system
{

void init(dword mem_low)
{
	kprintf("%t system::init: mem_low == %u .\n", mem_low);
}

void panic(char const * msg)
{
	kputs(msg);
	kputs("system::panic: kernel panic, system halted\n");
	asm volatile ( "cli\n\t" : : : ) ;
	for ( ; ; ) asm volatile ( "hlt\n\t" : : : ) ;
}

}
