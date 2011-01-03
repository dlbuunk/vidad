#include "io.h"

namespace IO
{	void Init::main()
	{	// init PIC
		PIC *pic;
		pic = new PIC();

		// init timer
		Timer *timer;
		timer = new Timer(100.0);

		// create VGA video output
		Video_Text *video;
		VGA_Text *vga_text;
		vga_text = new VGA_Text(4);
		video = vga_text;

		// open kernel terminal
		Term_Buf *term_buf;
		Term_Std *term_std;
		byte num_col = video->get_num_col(); // without this the
		byte num_row = video->get_num_row(); // constuctor fails
		term_buf = new Term_Buf(video, num_col, num_row);
		memcpyw((dword) term_buf->buffer, 2000, 0xB8000);
		term_buf->cursor_pos = kprint_pos;
		term_std = new Term_Std(term_buf, term_buf);
		kterm = term_std;
		kterm->set_active();

		// init keyboard
		ByteBuffer *scancode_buffer;
		KBC *kbc;
		scancode_buffer = new ByteBuffer(128);
		kbc = new KBC(scancode_buffer);

		for (;;) asm("hlt"); // halt the system

		// exit keyboard
		delete kbc;

		// close kernel terminal
		if (kterm == term_std)
		{	delete term_std;
			delete term_buf;
		}

		// close VGA video output
		if (vga_text == video) delete vga_text;

		// exit timer
		delete timer;

		// exit PIC
		delete pic;
	};
};
