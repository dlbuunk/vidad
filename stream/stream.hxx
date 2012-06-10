//	stream.hxx
//
//	Copyright 2012 D.L.Buunk <dlbuunk@gmail.com>
//
//	This file is part of ViOS.
//
//	ViOS is free software; you can redistribute it and/or modify
//	it under the terms of the GNU General Public License as published by
//	the Free Software Foundation; either version 2 of the License, or
//	(at your option) any later version.
//
//	ViOS is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//	GNU General Public License for more details.
//
//	You should have received a copy of the GNU General Public License
//	along with ViOS; if not, write to the Free Software
//	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
//	MA 02110-1301, USA.

#ifndef STREAM_HXX
#define STREAM_HXX

#include "kernel.hxx"
#include "util.hxx"
using util::strncpy;

#define DEV_ID_LEN 80

namespace stream
{

class Stream
{
	public:
	Stream()
	{
		s_id[0] = ';';
		s_size = 0;
	}
	virtual ~Stream() {}
	char const * get_id() { return s_id; }
	qword get_size() { return s_size; }
	virtual void flush() {};
	virtual bool eos() = 0;

	protected:
	// Yes, i know, this can lead to dangerous buffer overflows.
	char s_id[DEV_ID_LEN];
	qword s_size;
};

class Stream_s: public virtual Stream
{
	public:
	Stream_s()
	{
		s_pos = 0;
	}
	virtual ~Stream_s() {}
	virtual int seek(long int offset, int origin) = 0;
	qword get_pos() { return s_pos; }
	bool eos()
	{
		if (s_pos < s_size)
			return false;
		else
			return true;
	}

	protected:
	qword s_pos;
};

class Stream_r: public virtual Stream
{
	public:
	virtual ~Stream_r() {}
	virtual byte read() = 0;
	virtual size_t read(byte * p, size_t num) = 0;
};

class Stream_rs: public Stream_r, public Stream_s
{
	public:
	virtual ~Stream_rs() {}
};

class Stream_w: public virtual Stream
{
	public:
	virtual ~Stream_w() {}
	virtual void write(byte val) = 0;
	virtual size_t write(byte * p, size_t num) = 0;
};

class Stream_ws: public Stream_w, public Stream_s
{
	public:
	virtual ~Stream_ws() {}
};

class Stream_rwsb: public Stream_r, public Stream_w, public Stream_s
{
	public:
	virtual ~Stream_rwsb() {}
};

class Device
{
	public:
	Device(char const * to_dev_id, byte devtype)
	{
		type = devtype;
		open_write = false;
		open_read = 0;
		reg_subdev = 0;
		strncpy(dev_id, to_dev_id, DEV_ID_LEN);
	}
	byte get_type() { return type; }
	bool can_close()
	{
		if (open_read || open_write)
			return false;
		else
			return true;
	}

	virtual Stream_r * open_r() { return 0; }
	virtual Stream_rs * open_rs() { return 0; }
	virtual Stream_w * open_w() { return 0; }
	virtual Stream_ws * open_ws() { return 0; }
	virtual Stream_rwsb * open_rwsb() { return 0; }

	virtual Device * open_subdev(int num_subdev)
	{
		if (num_subdev = 0 && reg_subdev)
			return reg_subdev;
		else
			return 0;
	}

	int register_subdev(Device * dev)
	{
		if ((type & 0xE0) == 0x80)
		{
			reg_subdev = dev;
			return 0;
		}
		else
			return -1;
	}

	protected:
	byte type;
	bool open_write;
	unsigned int open_read;
	Device * reg_subdev;
	char dev_id[DEV_ID_LEN];
};

class Filesys: public Device
{
	public:
	Filesys(char const * dev_id, byte devtype) : Device(dev_id, devtype)
	{
		fopen_write = 0;
		fopen_read = 0;
	}

	// Ordinary lookup
	virtual char const * name_to_num(char const * name) = 0;
	virtual unsigned int name_to_num_raw(char const * name) = 0;
	// And the reverse lookup
	virtual char const * num_to_name(unsigned int num) = 0;
	// TODO: stat should be here too

	virtual Stream_r * open_r(unsigned int num) = 0;
	virtual Stream_rs * open_rs(unsigned int num) = 0;
	virtual Stream_w * open_w(unsigned int num) = 0;
	virtual Stream_ws * open_ws(unsigned int num) = 0;

	protected:
	unsigned int fopen_write;
	unsigned long long int fopen_read;
};

}

#endif // STREAM_HXX
