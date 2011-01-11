/* This file is part of Vidad.
*
*  Vidad is free software: you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  Vidad is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with Vidad.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <kernel.h>
#include <io/io.h>

#include <string>
using klib::string;

// TODO:
// make set_keyset() edit the command word, not overwrite it.
// prevent init() from chocking on no echo

// port 0x64 is command/status
// port 0x60 is data
namespace IO
{	KBC::KBC()
	{	tl = 0;
		error = 0;
		init();
	}

	void KBC::init()
	{	byte result;
		echo = ack = 0;
		string str(80);

		// get irq handle
		if ((inter_num = inter_reg((dword) &handle_irq, (dword) this, 0x01)) == -1)
		{	print("Keyboard, KBC: error, cannot get irq1 handle, keyboard won't function.\n");
		}

		// enable keyboard
		wait_write();
		outportb(0x64, 0xAE);
		send(0xF4);
		while (! ack);
		print("Keyboard, KBC: keyboard enabled.\n");

		// self test
		wait_write();
		outportb(0x64, 0xAA);
		wait_read();
		result = inportb(0x60);
		if (result == 0x55) print("Keyboard, KBC: self test OK.\n");
		else print(str.append("Keyboard, KBC: self test error, error code 0x").appendHex(result, 2).append('\n'));

		// interface test
		wait_write();
		outportb(0x64, 0xAB);
		wait_read();
		result = inportb(0x60);
		if (! result) print("Keyboard, KBC: interface test OK.\n");
		else print(str.clear().append("Keyboard, KBC: interface test error, error code 0x").appendHex(result, 2).append('\n'));

		// set command byte to 0x65, translate, keyboard, no mouse
		wait_write();
		outportb(0x64, 0x60);
		wait_write();
		outportb(0x60, 0x65);
		wait_write();
		outportb(0x64, 0x20);
		wait_read();
		result = inportb(0x60);
		if (result != 0x65) print(str.clear().append("Keyboard, KBC: error, cannot set command byte, reread value is: 0x").appendHex(result, 2).append('\n'));

		// echo
		send(0xEE);
		while (! echo); // This should be changed, if there is no echo, the thing get struck here.
		print("Keyboard, KBC: echo OK.\n");

		// set all keys to produce break codes.
		send(0xFA);

		// enable keyboard (second time)
		ack = 0;
		wait_write();
		outportb(0x64, 0xAE);
		send(0xF4);
		while (! ack);

		// clear all leds
		set_leds(0);
	}

	KBC::~KBC()
	{	// remove irq handle
		inter_dereg(0x01, inter_num);
		// disable keyboard
		wait_write();
		outportb(0x64, 0xAD);
		print("Keyboard, KBC: keyboard disabled.\n");
	}

	void KBC::wait_read()
	{	while (! (inportb(0x64) & 0x01));
	}

	void KBC::wait_write()
	{	while (inportb(0x64) & 0x02);
	}

	void KBC::send(byte val)
	{	wait_write();
		outportb(0x60, last_send = val);
	}

	void KBC::handle_irq(KBC *th)
	{	th->wait_read();
		switch (byte incode = inportb(0x60))
		{	case 0xEE : th->echo = 1; return;
			case 0xFA : th->ack = 1; return; // ACK
			case 0xFE : th->send(th->last_send); return; // NACK
			case 0x00 : // these are errors of some kind
			case 0xFC :
			case 0xFD :
			case 0xFF :
			{	if (th->error) print("Fatal keyboard error, keyboard now disfunct.\nPress any key, go ahead, it won't do anything now.");
				else
				{	print("Keyboard error, keyboard will try to re-init");
					th->error = 1;
					asm ("mov $0x20,%%al\n\tout %%al,$0x20\n\t" : : : "%al"); // ack the irq to the PIC
					th->init();
				}
			} break;
			default : if (th->tl) th->tl->feed_scancode(incode);
		}
	}

	void KBC::set_leds(byte status)
	{	send(0xED);
		send(status);
	}

	void KBC::set_translator(Key_Translate *new_tl)
	{	tl = new_tl;
	}

	void KBC::set_keyset(int set)
	{	if (set == 0)
		{	// translate ON
			wait_write();
			outportb(0x64, 0x60);
			wait_write();
			outportb(0x60, 0x65);
		}
		else
		{	// set scanset
			send(0xF0);
			send(set);
			// translate OFF
			wait_write();
			outportb(0x64, 0x60);
			wait_write();
			outportb(0x60, 0x25);
		}
	}

	void KBC::reset_cpu()
	{	byte outputport;
		wait_write();
		outportb(0x64, 0xD0);
		wait_read();
		outputport = inportb(0x60) & 0xFE;
		wait_write();
		outportb(0x64, 0xD1);
		wait_write();
		outportb(0x60, outputport);
		asm("cli");
		asm("hlt");
	}
}
