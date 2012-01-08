//      manager.cxx
//
//      Copyright 2012 D.L.Buunk <dlbuunk@gmail.com>
//
//	This file is part of ViOS
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
using util::kprintf;

#include "io.hxx"

namespace io
{

// Port I/O functions.
void outb(byte val, word port)
{
	asm volatile (
		"outb	%0,%1\n\t"
		:
		: "a" (val)
		, "Nd" (port)
		:
		) ;
}

void outw(word val, word port)
{
	asm volatile (
		"outw	%0,%1\n\t"
		:
		: "a" (val)
		, "Nd" (port)
		:
		) ;
}

void outd(dword val, word port)
{
	asm volatile (
		"outl	%0,%1\n\t"
		:
		: "a" (val)
		, "Nd" (port)
		:
		) ;
}

byte inb(word port)
{
	byte val;
	asm volatile (
		"inb	%1,%0\n\t"
		: "=a" (val)
		: "Nd" (port)
		:
		) ;
	return val;
}

word inw(word port)
{
	word val;
	asm volatile (
		"inw	%1,%0\n\t"
		: "=a" (val)
		: "Nd" (port)
		:
		) ;
	return val;
}

dword ind(word port)
{
	dword val;
	asm volatile (
		"inl	%1,%0\n\t"
		: "=a" (val)
		: "Nd" (port)
		:
		) ;
	return val;
}

void outsb(byte * ptr, word port, size_t count)
{
	asm volatile (
		"rep	outsb\n\t"
		:
		: "d" (port)
		, "S" (ptr)
		, "c" (count)
		:
		) ;
}

void outsw(word * ptr, word port, size_t count)
{
	asm volatile (
		"rep	outsw\n\t"
		:
		: "d" (port)
		, "S" (ptr)
		, "c" (count)
		:
		) ;
}

void outsd(dword * ptr, word port, size_t count)
{
	asm volatile (
		"rep	outsl\n\t"
		:
		: "d" (port)
		, "S" (ptr)
		, "c" (count)
		:
		) ;
}

void insb(byte * ptr, word port, size_t count)
{
	asm volatile (
		"rep	insb\n\t"
		:
		: "d" (port)
		, "D" (ptr)
		, "c" (count)
		:
		) ;
}

void insw(word * ptr, word port, size_t count)
{
	asm volatile (
		"rep	insw\n\t"
		:
		: "d" (port)
		, "D" (ptr)
		, "c" (count)
		:
		) ;
}

void insd(dword * ptr, word port, size_t count)
{
	asm volatile (
		"rep	insl\n\t"
		:
		: "d" (port)
		, "D" (ptr)
		, "c" (count)
		:
		) ;
}

}
