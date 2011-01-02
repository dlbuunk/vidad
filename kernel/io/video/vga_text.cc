#include "../io.h"

namespace IO
{	VGA_Text::VGA_Text(dword mode)
	{	// try to set to specified mode, on failure set to backup mode (mode 2)
		if (mode > 0xF) mode = 2;
		if (! set_mode(mode)) { mode = 2; set_mode(2); }
		switch (mode)
		{	case 2 :
			case 3 : set_font(system_font8, 8); break;
			case 4 :
			case 5 : set_font(system_font16, 16); break;
		}
	};

	VGA_Text::~VGA_Text()
	{
	};

	void VGA_Text::redraw(word *buffer, word cursor_pos)
	{	if (current_buffer != buffer) return;
		memcpyw(0xB8000, 2000, (dword) buffer);
		set_cursor(cursor_pos);
	};

	byte VGA_Text::get_num_col()
	{	switch (current_mode)
		{	case 2 :
			case 3 : return(40);
			case 4 :
			case 5 :
			case 6 :
			case 7 : return(80);
			case 8 :
			case 9 :
			case 10:
			case 11: return(90);
		}
		return(0);
	};
	byte VGA_Text::get_num_row()
	{	switch (current_mode)
		{	case 2 :
			case 3 :
			case 4 :
			case 5 :
			case 8 : return(25);
			case 9 : return(30);
			case 6 :
			case 7 :
			case 10: return(50);
			case 11: return(60);
		}
		return(0);
	};

	void VGA_Text::set_cursor(word cursor_pos)
	{	outportb(crtc_index, 0x0E);
		outportb(crtc_data, cursor_pos>>8);
		outportb(crtc_index, 0x0F);
		outportb(crtc_data, cursor_pos);
	};

	void VGA_Text::set_font(const byte *font, byte height)
	{	byte seq2, seq4, gc4, gc5, gc6;
		int i;

		// save & set seq2
		outportb(seq_index, 0x02);
		seq2 = inportb(seq_data);
		outportb(seq_data, 0x04);
		// save & modify seq4
		outportb(seq_index, 0x04);
		outportb(seq_data, (seq4 = inportb(seq_data)) | 0x04);
		// save & set gc4
		outportb(gc_index, 0x04);
		gc4 = inportb(gc_data);
		outportb(gc_data, 0x02);
		// save & modify gc5
		outportb(gc_index, 0x05);
		outportb(gc_data, (gc5 = inportb(gc_data)) & 0xEF);
		// save & modify gc6
		outportb(gc_index, 0x06);
		outportb(gc_data, (gc6 = inportb(gc_data)) & 0xFD);

		// write font data
		for (i=0; i<256; i++)
		{	memcpyb(0xB8000 + (i<<5), height, (dword) font);
			font += height;
		}

		// restore regs
		outportb(gc_index, 0x06);
		outportb(gc_data, gc6);
		outportb(gc_index, 0x05);
		outportb(gc_data, gc5);
		outportb(gc_index, 0x04);
		outportb(gc_data, gc4);
		outportb(seq_index, 0x04);
		outportb(seq_data, seq4);
		outportb(seq_index, 0x02);
		outportb(seq_data, seq2);
	};
};
