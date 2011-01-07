#include "../io.h"

// Keyboard control center, can be put in 3 modes:
// mode 1: relay everything untouched except Alt-SysReq and Control-Alt-Delete
// mode 2: ignore break codes, handle control and alt seperately, but don't handle shift and send ather control chars.
// mode 3: send pure ascii, handle shift.

// TODO: add handling of control, alt and F[1..12]

namespace IO
{	Keyboard::Keyboard()
	{	buffer = 0;
		shift_state = 0;
	};

	Keyboard::~Keyboard()
	{
	};

	void Keyboard::feed_code(word code)
	{	if (code == 0xD200) // control-alt-delete pressed, reset the CPU by triple-faulting
		{	asm("xorl %%eax,%%eax\n\tmovw %%ax,0x6008\n\tlidt 0x6008\n\tint $0xFF\n\t" : : : "%eax");
		}
		if ((code & ~0xC000) == 0x1100) // SysReq pressed, now, kill the kernel, later, handle it
		{	kerror("\nSYSREQ pressed, \"and you thought that it would rescue your system, think again.\"", 0x1F);
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
				{	shift_state = 1;
					return;
				}
				if ((code & 0x00FF) == 0x0E) // SO
				{	shift_state = 0;
					return;
				}
				if (code & 0xFF00) return;
				if (shift_state) code = keyboard_shift[code];
			} break;
		}
		if (buffer) buffer->write(code);
	};

	void Keyboard::set_user(WordBuffer *buffer, int new_mode)
	{	this->buffer = buffer;
		mode = new_mode;
	};
};
