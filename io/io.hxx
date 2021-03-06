//      io.hxx
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

#ifndef IO_HXX
#define IO_HXX

#include "kernel.hxx"
#include "stream.hxx"
using stream::Device;

namespace io
{

// Port I/O functions.
void outb(byte val, word port);
void outw(word val, word port);
void outd(dword val, word port);
byte inb(word port);
word inw(word port);
dword ind(word port);
void outsb(byte * ptr, word port, size_t count);
void outsw(word * ptr, word port, size_t count);
void outsd(dword * ptr, word port, size_t count);
void insb(byte * ptr, word port, size_t count);
void insw(word * ptr, word port, size_t count);
void insd(dword * ptr, word port, size_t count);

// Main devices and initialisation
extern Device * dev_root;
int init_dev();
int exit_dev();

}

#endif // IO_HXX
