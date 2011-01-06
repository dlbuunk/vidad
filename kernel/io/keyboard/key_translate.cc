#include "../io.h"

// TODO: handle escaped scancodes

namespace IO
{	Key_Translate_Set1::Key_Translate_Set1(Keyboard *out, KBC *kbc)
	{	this->kbc = kbc;
		kbc->set_translator(this);
		status = 0;
		this->out = out;
	};

	Key_Translate_Set1::~Key_Translate_Set1()
	{
	};

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
		{	switch (incode)
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
				case 0x53 : outcode = 0x122E; break; // DEL && .
				case 0x52 : outcode = 0x1330; break; // INS && 0
				case 0x47 : outcode = 0x1437; break; // HOME && 7
				case 0x4F : outcode = 0x1531; break; // END && 1
				case 0x49 : outcode = 0x1639; break; // Pup && 9
				case 0x51 : outcode = 0x1733; break; // Pdown && 3
				case 0x4B : outcode = 0x1834; break; // <- && 4
				case 0x4D : outcode = 0x1936; break; // -> && 6
				case 0x48 : outcode = 0x1A38; break; // Up && 8
				case 0x50 : outcode = 0x1B32; break; // Down && 2
				case 0x01 : outcode = 0x1C00; break; // ESC
			}
		}
		if ((status & 0x04) && outcode > 0x60 && outcode < 0x7B) outcode -= 0x20; // handle CAPS lock
		out->feed_code(outcode | ((status&0xE0)<<8));
	};
};
