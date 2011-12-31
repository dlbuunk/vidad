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
size_t strlen(char const *);
int strcmp(char const *, char const *);
int strncmp(char const *, char const *, size_t);
char * strcpy(char *, char const *);
char * strncpy(char *, char const *, size_t);
char * strcat(char *, char const *);
char * strncat(char *, char const *, size_t);

// from kprint.cxx
extern void (*loader_puts)(char const *);
void kputs(char const *);

}

#endif // UTIL_HXX
