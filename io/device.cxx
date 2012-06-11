//      device.cxx
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

#include "io.hxx"

namespace io
{

class Device_legacy: public Device
{
	public:
	Device_legacy() : Device("|0;", 0x40)
	{
		// TODO: init legacy devices.
	}
	~Device_legacy()
	{
		// TODO: exit legacy devices.
	}

	Device * open_subdev(int num_subdev)
	{
		// TODO: return legacy devices
		(void) num_subdev;
		return 0;
	}
};

class Device_root: public Device
{
	public:
	Device_root() : Device("|;", 0x40)
	{
		dev_legacy = new Device_legacy();
		// init PCI
	}
	~Device_root()
	{
		// exit PCI
		delete dev_legacy;
	}

	Device * open_subdev(int num_subdev)
	{
		switch (num_subdev)
		{
		case 0 : return dev_legacy;
		case 4 : // PCI
		default:
			return 0;
		}
	}

	private:
	Device * dev_legacy;
};

int init_dev()
{
	dev_root = new Device_root();
	return 0;
}

int exit_dev()
{
	delete dev_root;
	return 0;
}

Device * dev_root = 0;

}
