#include <kernel.h>
#include <io/io.h>

namespace IO
{	Init::Init() // IO init function, should start all devices
	{	// init MISC devices
		pic = new PIC();
		timer = new Timer(256.0);

		// create VGA video output
		video = vga_text = new VGA_Text(9);
		printf("VGA set to mode 9, 30 rows, 90 cols, 16 pixel high fonts.\n");

		// setup keyboard
		kbc = new KBC;
		kbc->set_keyset(2);
		kbc->set_keyset(0);
		keyb = new Keyboard;
		key_translate = key_trans1 = new Key_Translate_Set1(keyb, kbc);

		WordBuffer *key_buf;
		key_buf = new WordBuffer(64);
		keyb->set_user(key_buf, 3);

		// open kernel terminal
		byte num_col = video->get_num_col(); // without this the
		byte num_row = video->get_num_row(); // constuctor fails

		term_buf = new Term_Buf(video, num_col, num_row); // open buffer
		for (int i=0; i<25; i++) memcpyw((word *) term_buf->buffer + 90*i, (word *) 0xB8000 + 80*i, 80); // copy video memory to buffer
		term_buf->cursor_pos = (kprint_pos/80)*90; // and set the cursor position with the value kprint ended with
		term_buf->vid->current_buffer = term_buf->buffer; // set active
		term_buf->vid->redraw(term_buf->buffer, term_buf->cursor_pos); // redraw

		kterm = term_std = new Term_Std(term_buf, term_buf, key_buf); // open terminal with both stderr and stdout set to the same buffer
		kterm->set_color(0x0E); // yellow
		kterm->puts("Kernel terminal active.\n"); // and test it
		kterm->set_color(0x07); // white
	}
	
	Init::~Init() // IO exit function, should run everything in Init::Init() in REVERSE ORDER!
	{	// close kernel terminal
		if (kterm == term_std)
		{	delete term_std;
			delete term_buf;
		}

		// close VGA video output
		if (vga_text == video) delete vga_text;

		// close misc devices
		delete timer;
		delete pic;
	}
}
