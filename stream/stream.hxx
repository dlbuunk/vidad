//      stream.hxx
//
//      Copyright 2012 D.L.Buunk <dlbuunk@gmail.com>
//
//	This file is part of ViOS.
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

#ifndef STREAM_HXX
#define STREAM_HXX

#include "kernel.h"
#include "util.h"

namespace stream
{

enum seek_origin
{
	set,
	cur,
	end
};

class Stream_r:
{
	public:
	virtual byte read() = 0;
	virtual size_t read(byte * ptr, size_t num) = 0;
	virtual bool eos() = 0;
	virtual void flush() {};
};

class Stream_rs: public Stream_r
{
	public:
	virtual int seek_r(long int offset, seek_origin origin) = 0;
};

class Stream_w:
{
	public:
	virtual void write(byte val) = 0;
	virtual size_t write(byte * ptr, size_t num) = 0;
	virtual bool eos() = 0;
	virtual void flush() {};
};

class Stream_ws: public Stream_w
{
	public:
	virtual int seek_w(long int offset, seek_origin origin) = 0;
};

// This class is only intended for low-level devices which cannot seek
// for reading and writing independently.
// Note that not all seek_origin values need be supported, though one of
// them will certainly work.
class Stream_rwsb: virtual public Stream_r, virtual public Stream_w
{
	public:
	virtual int seek_rw(long int offset, seek_origin origin) = 0;
};

// And this might be used for a ordinary read/write stream.
// It is only available for 'full' rws streams, which are supposed
// to be able to seek independently, heck, they don't even need be
// connected to the same Stream object!
struct Stream_rws
{
	Stream_rs * r;
	Stream_ws * w;
};

// Some ascii convertors, does this seem okay?
class Stream_ra
{
	public:
	Stream_ra(Stream_r *s)
	{
		str_ptr = s;
	}
	int read(char *);
	private:
	Stream_r * str_ptr;
};

class Stream_wa
{
	public:
	Stream_ra(Stream_w *s)
	{
		str_ptr = s;
	}
	int write(char *);
	private:
	Stream_w * str_ptr;
};

}

#endif // STREAM_HXX
