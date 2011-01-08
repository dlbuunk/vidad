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
	{
	}

	void Key_Translate_Set2::feed_scancode(byte incode)
	{	word outcode;

		// if (code == 0xF0) then next code is break code
		if (incode == 0xF0) { status |= 0x20; return; }

		// escape code 1
		if (incode == 0xE1) { status |= 0x10; return; }

		// escape code 0
		if (incode == 0xE0) { status |= 0x08; return; }

		if (status & 0x08) // handle escape 0
		{
			status &= ~0x08; // turn off escape flag
		}

		else if (status & 0x10) // handle escape 1
		{
			status &= ~0x10; // turn off escape flag
		}

		else // handle normal key
		{	switch (incode)
			{	default : outcode = (word) keyboard_mode2[incode];
			}
		}

		outcode |= (status & 0xE) << 8;
		status &= ~0x20; // turn off break flag if it happens to be on
		out->feed_code(outcode);
	}
}
