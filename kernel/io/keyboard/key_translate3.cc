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

namespace IO
{	Key_Translate_Set3::Key_Translate_Set3(Keyboard *out, KBC *kbc)
	{	this->kbc = kbc;
		this->out = out;
		kbc->set_translator(this);
		status = 0;
	}

	Key_Translate_Set3::~Key_Translate_Set3()
	{	kbc->set_translator(NULL); // give the kbc a NULL pointer, so it won't call an nonexistent object.
	}

	void Key_Translate_Set3::feed_scancode(byte incode)
	{	word outcode = 0;

		// if (code == 0xF0) then next code is break code
		if (incode == 0xF0) { status |= 0x20; return; }

		switch (incode)
		{	case 0x5F : if (status & 0x20) { status ^= 0x01; kbc->set_leds(status & 0x07); } goto end; // scroll lock
			case 0x76 : if (status & 0x20) { status ^= 0x02; kbc->set_leds(status & 0x07); } goto end; // num lock
			case 0x14 : if (status & 0x20) { status ^= 0x04; kbc->set_leds(status & 0x07); } goto end; // CAPS lock
			case 0x12 : // shift
			case 0x59 : outcode = status & 0x20 ? 0x000E : 0x000F; break;
			case 0x11 : // control
			case 0x58 : status = (status & 0x20) ? (status & ~0x80) : (status | 0x80); goto end;
			case 0x8D : // APPS key
			case 0x8B : // win key
			case 0x8C : goto end;
			case 0x19 : // alt
			case 0x39 : status = (status & 0x20) ? (status & ~0x40) : (status | 0x40); goto end;
			case 0x57 : outcode = status & 0x40 ? 0x1100 : 0x1000; break; // print-screen/SysRq
			case 0x84 : incode = 0x4E; // keypad -
			default : outcode = keyboard_mode3[incode];
		}

		if (! (status & 0x02)) switch (incode) // handle no-num lock
		{	case 0x71 : outcode = 0x1200; break;
			case 0x70 : outcode = 0x1300; break;
			case 0x69 : outcode = 0x1500; break;
			case 0x72 : outcode = 0x1B00; break;
			case 0x7A : outcode = 0x1700; break;
			case 0x6B : outcode = 0x1800; break;
			case 0x73 : goto end;
			case 0x74 : outcode = 0x1900; break;
			case 0x6C : outcode = 0x1400; break;
			case 0x75 : outcode = 0x1A00; break;
			case 0x7D : outcode = 0x1600; break;
		}

		if ((status & 0x04) && outcode > 0x60 && outcode < 0x7B) outcode -= 0x20; // handle caps-lock
		out->feed_code(outcode | ((status & 0xE0) << 8));
		end: status &= ~0x20; // break flag off
	}
}
