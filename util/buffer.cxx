//      buffer.cxx
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

namespace util
{

Buffer::Buffer(size_t n)
{
	b = new byte[n];
	size = n;
	ri = wi = 0;
}
Buffer::~Buffer()
{
	delete b;
}

byte Buffer::r()
{
	if (! cr())
		return 0xFF;
	byte rv = b[ri++];
	if (ri == size)
		ri = 0;
	return rv;
}

int Buffer::r(byte * p, size_t n)
{
	size_t num = 0;
	while (cr())
	{
		*p++ = r();
		num++;
		if (n == num)
			break;
	}
	return num;
}

bool Buffer::cr()
{
	if (ri == wi)
		return false;
	return true;
}

int Buffer::w(byte v)
{
	if (! cw())
		return -1;
	b[wi++] = v;
	if (wi == size)
		wi = 0;
	return 0;
}

int Buffer::w(byte const * p, size_t n)
{
	size_t num = 0;
	while (cw())
	{
		w(*p++);
		num++;
		if (num == n)
			break;
	}
	return num;
}

bool Buffer::cw()
{
	if (ri - wi == 1 || (! ri && size - wi == 1))
		return false;
	return true;
}

}
