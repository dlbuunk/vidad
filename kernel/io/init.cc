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

		// this should print !s on the screen with a freq of 1 Hz
		for (;;)
		{	timer->reg_cb((dword) &test, (dword) this, 100);
			lock = 1;
			while (lock) asm("hlt"); //cool down cpu
		}

		// close kernel terminal
		if (kterm == term_std)
		{	delete term_std;
			delete term_buf;
		}

		// close VGA video output
		// commented out because it would clear the screen
		if (vga_text == video) delete vga_text;

		// exit timer
		delete timer;

		// exit PIC
		delete pic;
	};

	void Init::test(dword ptr)
	{	Init *th;
		th = (Init *) ptr;
		printf("!");
		th->lock = 0;
	};
};
