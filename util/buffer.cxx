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

Buffer::Buffer()
{
	first = new BufferBlock();
	first->next = 0;
	rp = wp = first->data;
	last = first;
}

Buffer::~Buffer()
{
	BufferBlock * temp;
	while (first != last)
	{
		temp = first;
		first = first->next;
		delete temp;
	}
	delete last;
}

size_t Buffer::w(byte const * p, size_t n)
{
	size_t num = n;
	while (num + (wp - last->data) >= buf_size)
	{
		memcpy(wp, p, last->data - wp + buf_size);
		p += last->data - wp + buf_size;
		num -= last->data - wp + buf_size;
		last->next = new BufferBlock();
		last = last->next;
		wp = last->data;
	}
	memcpy(wp, p, num);
	wp += num;
	return n;
}

void Buffer::w(byte v)
{
	w(&v, 1);
}

size_t Buffer::r(byte *p, size_t n)
{
	size_t num = n;
	while (first != last && num + (rp - first->data) >= buf_size)
	{
		memcpy(p, rp, buf_size + first->data - rp);
		p += first->data - rp + buf_size;
		num -= first->data - rp + buf_size;
		BufferBlock * temp = first;
		first = first->next;
		delete temp;
		rp = first->data;
	}
	if (first != last)
	{
		memcpy(p, rp, num);
		rp += num;
		return n;
	}
	if (wp < rp + num)
	{
		memcpy(p, rp, wp - rp);
		num -= wp - rp;
		rp = wp;
		return n - num;
	}
	memcpy(p, rp, num);
	rp += num;
	return n;
}

byte Buffer::r()
{
	byte b;
	r(&b, 1);
	return b;
}

}
