//      system.hxx
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

#ifndef SYSTEM_HXX
#define SYSTEM_HXX

#include "kernel.hxx"

namespace system
{

void init_nt(dword mem_low);
void init(void * mem_low_p);
// mem_low being the only bootloader non-fixed address parameter that still
// needs to be distributed to it's final place...

void panic(char const * msg);

}

#endif // SYSTEM_HXX
