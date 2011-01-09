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

// Keyboard control center, can be put in 3 modes:
// mode 1: relay everything untouched except Alt-SysReq and Control-Alt-Delete
// mode 2: ignore break codes, handle control and alt seperately, but don't handle shift and send ather control chars.
// mode 3: send pure ascii, handle shift.

// TODO: add handling of control, alt and F[1..12]

namespace IO
{	Keyboard::Keyboard()
	{	buffer = 0;
		shift_state = 0;
	}

	Keyboard::~Keyboard()
	{
	}

	void Keyboard::feed_code(word code)
	{	if (code == 0xD200) // control-alt-delete pressed, reset the CPU by triple-faulting
		{	asm("xorl %%eax,%%eax\n\tmovw %%ax,0x6008\n\tlidt 0x6008\n\tint $0xFF\n\t" : : : "%eax");
		}
		if ((code & ~0xC000) == 0x1100) // SysReq pressed, for now, reset the shift state.
		{	shift_state = 0;
			return;
		}
		switch (mode)
		{	case 1 : break;
			case 2 :
			{	if (code & 0xE000) return;
				if ((! (code & 0x1000)) && (code & 0x0F00)) return; // test for function keys
			} break;
			case 3 :
			{	
				if ((code & 0x00FF) == 0x0F) // SI
				{	shift_state++;
					return;
				}
				if ((code & 0x00FF) == 0x0E) // SO
				{	shift_state--;
					return;
				}
				if (code & 0xFF00) return;
				if (shift_state) code = keyboard_shift[code];
			} break;
		}
		if (buffer) buffer->write(code);
	}

	void Keyboard::set_user(WordBuffer *buffer, int new_mode)
	{	this->buffer = buffer;
		mode = new_mode;
	}
}
