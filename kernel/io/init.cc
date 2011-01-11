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
{	Init::Init() // IO init function, should start all devices
	{	// init MISC devices
		pic = new PIC();
		timer = new Timer(256.0);

		// create VGA video output
		video = vga_text = new VGA_Text(9);
		print("VGA set to mode 9, 30 rows, 90 cols, 16 pixel high fonts.\n");

		// setup keyboard
		kbc = new KBC;
		kbc->set_keyset(3);
		keyb = new Keyboard;
		key_translate = key_trans3 = new Key_Translate_Set3(keyb, kbc);

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

		// close keyboard
		delete keyb;
		delete key_buf;
		if (key_translate == key_trans1) delete key_trans1;
		if (key_translate == key_trans2) delete key_trans2;
		if (key_translate == key_trans3) delete key_trans3;
		delete kbc;

		// close VGA video output
		if (vga_text == video) delete vga_text;

		// close misc devices
		delete timer;
		delete pic;
	}
}
