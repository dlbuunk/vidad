//      util.hxx
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

#ifndef UTIL_HXX
#define UTIL_HXX

#include "../kernel.hxx"

namespace util
{

// from string.cxx
size_t strlen(char const * str);
int strcmp(char const * s1, char const * s2);
int strncmp(char const * s1, char const * s2, size_t num);
char * strcpy(char * dest, char const * src);
char * strncpy(char * dest, char const * src, size_t num);
char * strcat(char * dest, char const * src);
char * strncat(char * dest, char const * src, size_t num);

// from kprint.cxx
extern void (*_loader_puts)(char const *);
void _kprints(char const * str);
void _format_str(char const * fstr, char * ostr, dword * args);
void kputs(char const * str);
int kprintf(char const * fstr, ...);
int sprintf(char * ostr, char const * fstr, ...);
}

#endif // UTIL_HXX
