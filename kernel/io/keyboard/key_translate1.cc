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

// NOTE:
// the fact that this code looks messy is because scancode set 1 IS messy!

// BUGS:
// printscreen is turned into an 8
// pause functions as a second num lock

namespace IO
{	Key_Translate_Set1::Key_Translate_Set1(Keyboard *out, KBC *kbc)
	{	this->kbc = kbc;
		kbc->set_translator(this);
		status = 0;
		this->out = out;
	}

	Key_Translate_Set1::~Key_Translate_Set1()
	{	kbc->set_translator(NULL); // give the kbc a NULL pointer, so it won't call an nonexistent object.
	}

	void Key_Translate_Set1::feed_scancode(byte incode)
	{	word outcode = 0;
		switch (incode)
		{	case 0x1D : status |= 0x80; return; // control IN
			case 0x38 : status |= 0x40; return; // alt IN
			case 0x3A : { status ^= 0x04; kbc->set_leds(status & 0x07); } return; // caps lock
			case 0x45 : { status ^= 0x02; kbc->set_leds(status & 0x07); } return; // num lock
			case 0x46 : { status ^= 0x01; kbc->set_leds(status & 0x07); } return; // scroll lock
			case 0x9D : status &= ~0x80; return; // control OUT
			case 0xAA : outcode = 0x000E; break; // left shift OUT
			case 0xB6 : outcode = 0x000E; break; // right shift OUT
			case 0xB8 : status &= ~0x40; return; // alt OUT
			case 0xE0 : status |= 0x08; return; // escape scancode
			default: break;
		}
		if (! outcode)
		{	if (incode & 0x80)
			{	status |= 0x20;
				incode &= ~0x80;
			}
			else status &= ~0x20;
			outcode = (word) keyboard_mode1[incode];
		}
		if (! outcode)
		{	if (! (status & 0x02)) switch (incode) // No NUM
			{	case 0x3B : outcode = 0x0100; break; // F1
				case 0x3C : outcode = 0x0200; break; // F2
				case 0x3D : outcode = 0x0300; break; // F3
				case 0x3E : outcode = 0x0400; break; // F4
				case 0x3F : outcode = 0x0500; break; // F5
				case 0x40 : outcode = 0x0600; break; // F6
				case 0x41 : outcode = 0x0700; break; // F7
				case 0x42 : outcode = 0x0800; break; // F8
				case 0x43 : outcode = 0x0900; break; // F9
				case 0x44 : outcode = 0x0A00; break; // F10
				case 0x57 : outcode = 0x0B00; break; // F11
				case 0x58 : outcode = 0x0C00; break; // F12
				case 0x37 : outcode = 0x1000; break; // printscreen
				case 0x54 : outcode = 0x1100; break; // SysReq
				case 0x53 : outcode = 0x1200; break; // DEL
				case 0x52 : outcode = 0x1300; break; // INS
				case 0x47 : outcode = 0x1400; break; // HOME
				case 0x4F : outcode = 0x1500; break; // END
				case 0x49 : outcode = 0x1600; break; // Pup
				case 0x51 : outcode = 0x1700; break; // Pdown
				case 0x4B : outcode = 0x1800; break; // <-
				case 0x4D : outcode = 0x1900; break; // ->
				case 0x48 : outcode = 0x1A00; break; // Up
				case 0x50 : outcode = 0x1B00; break; // Down
				case 0x01 : outcode = 0x1C00; break; // ESC
			}
			if ((! (status & 0x08)) && (status & 0x02)) switch (incode) // NUM, no escape
			{	case 0x3B : outcode = 0x0100; break; // F1
				case 0x3C : outcode = 0x0200; break; // F2
				case 0x3D : outcode = 0x0300; break; // F3
				case 0x3E : outcode = 0x0400; break; // F4
				case 0x3F : outcode = 0x0500; break; // F5
				case 0x40 : outcode = 0x0600; break; // F6
				case 0x41 : outcode = 0x0700; break; // F7
				case 0x42 : outcode = 0x0800; break; // F8
				case 0x43 : outcode = 0x0900; break; // F9
				case 0x44 : outcode = 0x0A00; break; // F10
				case 0x57 : outcode = 0x0B00; break; // F11
				case 0x58 : outcode = 0x0C00; break; // F12
				case 0x37 : outcode = 0x1000; break; // printscreen
				case 0x54 : outcode = 0x1100; break; // SysReq
				case 0x4C : outcode = 0x0035; break; // 5
				case 0x53 : outcode = 0x002E; break; // .
				case 0x52 : outcode = 0x0030; break; // 0
				case 0x47 : outcode = 0x0037; break; // 7
				case 0x4F : outcode = 0x0031; break; // 1
				case 0x49 : outcode = 0x0039; break; // 9
				case 0x51 : outcode = 0x0033; break; // 3
				case 0x4B : outcode = 0x0034; break; // 4
				case 0x4D : outcode = 0x0036; break; // 6
				case 0x48 : outcode = 0x0038; break; // 8
				case 0x50 : outcode = 0x0032; break; // 2
			}
			if ((status & 0x08) && (status & 0x02)) switch (incode) // NUM, escape
			{	case 0x3B : outcode = 0x0100; break; // F1
				case 0x3C : outcode = 0x0200; break; // F2
				case 0x3D : outcode = 0x0300; break; // F3
				case 0x3E : outcode = 0x0400; break; // F4
				case 0x3F : outcode = 0x0500; break; // F5
				case 0x40 : outcode = 0x0600; break; // F6
				case 0x41 : outcode = 0x0700; break; // F7
				case 0x42 : outcode = 0x0800; break; // F8
				case 0x43 : outcode = 0x0900; break; // F9
				case 0x44 : outcode = 0x0A00; break; // F10
				case 0x57 : outcode = 0x0B00; break; // F11
				case 0x58 : outcode = 0x0C00; break; // F12
				case 0x37 : outcode = 0x1000; break; // printscreen
				case 0x54 : outcode = 0x1100; break; // SysReq
				case 0x53 : outcode = 0x1200; break; // DEL
				case 0x52 : outcode = 0x1300; break; // INS
				case 0x47 : outcode = 0x1400; break; // HOME
				case 0x4F : outcode = 0x1500; break; // END
				case 0x49 : outcode = 0x1600; break; // Pup
				case 0x51 : outcode = 0x1700; break; // Pdown
				case 0x4B : outcode = 0x1800; break; // <-
				case 0x4D : outcode = 0x1900; break; // ->
				case 0x48 : outcode = 0x1A00; break; // Up
				case 0x50 : outcode = 0x1B00; break; // Down
				case 0x01 : outcode = 0x1C00; break; // ESC
			}
		}
		status &= ~ 0x08; // escape off
		if (! outcode) return;
		if ((status & 0x04) && outcode > 0x60 && outcode < 0x7B) outcode -= 0x20; // handle CAPS lock
		out->feed_code(outcode | ((status&0xE0)<<8));
	}
}
