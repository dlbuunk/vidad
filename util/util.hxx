//      util.hxx
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


// TODO: optimize memcpy/memset

#ifndef UTIL_HXX
#define UTIL_HXX

#include "kernel.hxx"

namespace util
{


// memory function templates
template <typename T>
int memcmp(T const * m1, T const * m2, size_t num)
{
	if (! num)
		return 0;
	for (size_t i=0; i<(num-1) && ! (*m1-*m2); i++)
		m1++, m2++;
	return *m1 - *m2;
}

template <typename T>
T * memcpy(T * dest, T const * src, size_t num)
{
	T * orig_ptr = dest;
	if (! num)
		return dest;
	for (size_t i=0; i<num; i++)
		*dest++ = *src++;
	return orig_ptr;
}

template <typename T>
T * memset(T * mem, T val, size_t num)
{
	T * orig_ptr = mem;
	if  (! num)
		return mem;
	for (size_t i=0; i<num; i++)
		*mem++ = val;
	return orig_ptr;
}

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

// from buffer.cxx
class Buffer
{
	public:

	Buffer(size_t n);
	~Buffer();

	byte r();
	int r(byte * p, size_t n);
	bool cr();
	int w(byte v);
	int w(byte const * p, size_t n);
	bool cw();

	private:
	byte * b;
	size_t ri, wi, size;
};

}

#endif // UTIL_HXX
