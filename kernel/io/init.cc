#include "io.h"

// kernel terminal, is global!
extern IO::Terminal *kterm;

namespace IO
{	void Init::main()
	{	// init PIC
		PIC *pic;
		pic = new PIC();

		// init timer
	//	Timer *timer;
	//	timer = new Timer(100.0);

		// create VGA video output
		Video_Text *video;
		VGA_Text *vga_text;
		vga_text = new VGA_Text(3);
		video = vga_text;

		// open kernel terminal
		Term_Buf *term_buf;
		Term_Std *term_std;
		byte num_col = video->get_num_col(); // without this the
		byte num_row = video->get_num_row(); // constuctor fails
		term_buf = new Term_Buf(video, num_col, num_row);
		term_std = new Term_Std(term_buf, term_buf);
		kterm = term_std;
		kterm->set_active();

		// output to screen
		kterm->set_color(BLUE);
		kterm->puts("SYSTEM LOADED\n");
		kterm->set_color(W);

		// pointer test
		dword ptr;
		asm ("movl %1,%%eax\n\tmovl %%eax,%0\n\t" : "=r" (ptr) : "r" (&IO::Init::test) : "%eax");
		printf("Pointer test:\n0x%8X\n0x%8X\n", ptr, (dword) this);

		// close kernel terminal
		if (kterm == term_std)
		{	delete term_std;
			delete term_buf;
		}

		// close VGA video output
		// commented out because it would clear the screen
	//	if (vga_text == video) delete vga_text;

		// exit timer
	//	delete timer;

		// exit PIC
		delete pic;
	};

	void Init::test()
	{	printf("!");
		lock = 0;
	};
};
