//      string.cxx
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

#include "util.hxx"

namespace util
{

size_t strlen(char const * str)
{
	size_t len = 0;
	while (*str)
		len++, str++;
	return len;
}

int strcmp(char const * s1, char const * s2)
{
	while (*s1 && ! (*s1 - *s2))
		s1++, s2++;
	return *s1 - *s2;
}

int strncmp(char const * s1, char const * s2, size_t num)
{
	for (size_t i=0; i<num && *s1 && ! (*s1 - *s2); i++)
		s1++, s2++;
	return *s1 - *s2;
}

char * strcpy(char * dest, char const * src)
{
	char * str_out = dest;
	while (*src)
		*dest++ = *src++;
	return str_out;
}

char * strncpy(char * dest, char const * src, size_t num)
{
	char * str_out = dest;
	for (size_t i=0; i<num && *src; i++)
		*dest++ = *src++;
	return str_out;
}

char * strcat(char * dest, char const * src)
{
	char * str_out = dest;
	while (*dest)
		dest++;
	while (*src)
		*dest++ = *src++;
	return str_out;
}

char * strncat(char * dest, char const * src, size_t num)
{
	char * str_out = dest;
	while (*dest)
		dest++;
	for (size_t i=0; i<num && *src; i++)
		*dest++ = *src++;
	return str_out;
}

}
