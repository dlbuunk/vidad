//      int_entry.cxx
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

//	All execptions, interrupt requests and system calls are send to
//	this file by the handlers in kernel_entry.s .
//	Although this code currently only prints a crude error message,
//	it is later expected to handle exceptions and rout irq's and syscalls
//	to the appropriate handlers elsewhere.

// General typedefs
typedef unsigned char byte;
typedef unsigned short int word;
typedef unsigned int dword;
typedef unsigned long long int qword;

// Main function
extern "C" void __int_entry(dword int_num)
{
	char msg[] = { "U n h a n d l e d   i n t e r r u p t . \0" } ;
	byte * msgp = (byte *) &msg[0];
	byte * video_mem = (byte *) 0x000B8000;
	while (*msgp)
		*video_mem++ = *msgp++;
	asm volatile ( "hlt\n\t" ) ;
	(void) int_num;
}
