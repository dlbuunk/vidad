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
{	Key_Translate_Set2::Key_Translate_Set2(Keyboard *out, KBC *kbc)
	{	this->kbc = kbc;
		this->out = out;
		kbc->set_translator(this);
		status = 0;
	}

	Key_Translate_Set2::~Key_Translate_Set2()
	{	kbc->set_translator(NULL); // give the kbc a NULL pointer, so it won't call an nonexistent object.
	}

	void Key_Translate_Set2::feed_scancode(byte incode)
	{	word outcode = 0;

		// if (code == 0xF0) then next code is break code
		if (incode == 0xF0) { status |= 0x20; return; }

		// escape code 1
		if (incode == 0xE1) { status |= 0x10; goto end; }

		// escape code 0
		if (incode == 0xE0) { status |= 0x08; goto end; }

		if (status & 0x08) // handle escape 0
		{	switch (incode)
			{	case 0x14 : status = (status & 0x20) ? (status & ~0x80) : (status | 0x80); break; // right control
				case 0x11 : status = (status & 0x20) ? (status & ~0x40) : (status | 0x40); break; // right alt
				case 0x5A : outcode = 0x000A; break; // keypad-enter
				case 0x12 : // fake shifts
				case 0x59 :
				{	outcode = (status & 0x20) ? 0x000E : 0x000F;
					status &= ~0x20; // turn off break flag
				} break;
				case 0x7C : outcode = 0x1000; break; // printscreen
				case 0x71 : outcode = 0x1200; break; // delete
				case 0x70 : outcode = 0x1300; break; // insert
				case 0x69 : outcode = 0x1500; break; // end
				case 0x72 : outcode = 0x1B00; break; // down
				case 0x7A : outcode = 0x1700; break; // Pdown
				case 0x6B : outcode = 0x1800; break; // left
				case 0x74 : outcode = 0x1900; break; // right
				case 0x6C : outcode = 0x1400; break; // home
				case 0x75 : outcode = 0x1A00; break; // up
				case 0x7D : outcode = 0x1600; break; // Pup
			}
			status &= ~0x08; // turn off escape flag
		}

		else if (status & 0x10) // handle escape 1
		{	switch (incode)
			{	case 0x14 : return;
				case 0x77 : outcode = 0x1D00;
				default : status &= ~0x10; // turn off escape flag
			}
			
		}

		else // handle normal key
		{	switch (incode)
			{	case 0x14 : status = (status & 0x20) ? (status & ~0x80) : (status | 0x80); break; // left control
				case 0x11 : status = (status & 0x20) ? (status & ~0x40) : (status | 0x40); break; // left alt
				case 0x59 : // right shift
				case 0x12 : // left shift, SO if break flag, SI otherwise
				{	outcode = (status & 0x20) ? 0x000E : 0x000F;
					status &= ~0x20; // turn off break flag
				} break;
				case 0x58 : if (status & 0x20) { status ^= 0x04; kbc->set_leds(status & 0x07); } goto end; // CAPS lock
				case 0x77 : if (status & 0x20) { status ^= 0x02; kbc->set_leds(status & 0x07); } goto end; // Num lock
				case 0x7E : if (status & 0x20) { status ^= 0x01; kbc->set_leds(status & 0x07); } goto end; // Scroll lock
				case 0x76 : outcode = 0x1C00; break; // Escape
				case 0x84 : outcode = 0x1100; goto feed; // sysrq, the goto is there to prevent that alt being send with.
				// here come the function keys
				case 0x05 : outcode = 0x0100; break;
				case 0x06 : outcode = 0x0200; break;
				case 0x04 : outcode = 0x0300; break;
				case 0x0C : outcode = 0x0400; break;
				case 0x03 : outcode = 0x0500; break;
				case 0x0B : outcode = 0x0600; break;
				case 0x83 : outcode = 0x0700; break;
				case 0x0A : outcode = 0x0800; break;
				case 0x01 : outcode = 0x0900; break;
				case 0x09 : outcode = 0x0A00; break;
				case 0x78 : outcode = 0x0B00; break;
				case 0x07 : outcode = 0x0C00; break;
				// and the keypad numbers (first the '.' then the numbers from 0 thru 9)
				case 0x71 : outcode = status & 0x02 ? 0x002E : 0x1200; break;
				case 0x70 : outcode = status & 0x02 ? 0x0030 : 0x1300; break;
				case 0x69 : outcode = status & 0x02 ? 0x0031 : 0x1500; break;
				case 0x72 : outcode = status & 0x02 ? 0x0032 : 0x1B00; break;
				case 0x7A : outcode = status & 0x02 ? 0x0033 : 0x1700; break;
				case 0x6B : outcode = status & 0x02 ? 0x0034 : 0x1800; break;
				case 0x73 : outcode = status & 0x02 ? 0x0035 : 0x0000; break;
				case 0x74 : outcode = status & 0x02 ? 0x0036 : 0x1900; break;
				case 0x6C : outcode = status & 0x02 ? 0x0037 : 0x1400; break;
				case 0x75 : outcode = status & 0x02 ? 0x0038 : 0x1A00; break;
				case 0x7D : outcode = status & 0x02 ? 0x0039 : 0x1600; break;
				default : outcode = (word) keyboard_mode2[incode];
			}
		}

		if ((status & 0x04) && outcode > 0x60 && outcode < 0x7B) outcode -= 0x20; // handle CAPS lock
		if (! outcode) goto end; // outcode == 0 means, we cannot use the code somehow.
		outcode |= (status & 0xE0) << 8;
		feed: out->feed_code(outcode);
		end: status &= ~0x20; // turn off break flag if it happens to be on
	}
}
