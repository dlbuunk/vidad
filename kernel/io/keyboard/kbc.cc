#include <kernel.h>
#include <io/io.h>

// TODO:
// make set_keyset() edit the command word, not overwrite it.
// prevent init() from chocking on no echo

// port 0x64 is command/status
// port 0x60 is data
namespace IO
{	KBC::KBC()
	{	tl = 0;
		error = 0;
		init();
	}

	void KBC::init()
	{	byte result;
		echo = 0;

		// enable keyboard
		wait_write();
		outportb(0x64, 0xAE);
		printf("Keyboard, KBC: keyboard enabled.\n");

		// self test
		wait_write();
		outportb(0x64, 0xAA);
		wait_read();
		result = inportb(0x60);
		if (result == 0x55) printf("Keyboard, KBC: self test OK.\n");
		else printf("Keyboard, KBC: self test error, error code 0x%2X.\n", result);

		// interface test
		wait_write();
		outportb(0x64, 0xAB);
		wait_read();
		result = inportb(0x60);
		if (! result) printf("Keyboard, KBC: interface test OK.\n");
		else printf("Keyboard, KBC: interface test error, error code 0x%2X.\n", result);

		// get irq handle
		if ((inter_num = inter_reg((dword) &handle_irq, (dword) this, 0x01)) == -1)
		{	printf("Keyboard, KBC: error, cannot get irq1 handle, keyboard won't function.\n");
		}

		// set command byte to 0x65, translate, keyboard, no mouse
		wait_write();
		outportb(0x64, 0x60);
		wait_write();
		outportb(0x60, 0x65);
		wait_write();
		outportb(0x64, 0x20);
		wait_read();
		result = inportb(0x60);
		if (result != 0x65) printf("Keyboard, KBC: error, cannot set command byte, reread value is: 0x%2X.\n", result);

		// echo
		send(0xEE);
		while (! echo); // This should be changed, if there is no echo, the thing get struck here.
		printf("Keyboard, KBC: echo OK.\n");

		set_leds(0);
	}

	KBC::~KBC()
	{	// remove irq handle
		inter_dereg(0x01, inter_num);
		// disable keyboard
		wait_write();
		outportb(0x64, 0xAD);
		printf("Keyboard, KBC: keyboard disabled.\n");
	}

	void KBC::wait_read()
	{	while (! (inportb(0x64) & 0x01));
	}

	void KBC::wait_write()
	{	while (inportb(0x64) & 0x02);
	}

	void KBC::send(byte val)
	{	wait_write();
		outportb(0x60, last_send = val);
	}

	void KBC::handle_irq(KBC *th)
	{	th->wait_read();
		switch (byte incode = inportb(0x60))
		{	case 0xEE : th->echo = 1; return;
			case 0xFA : return; // ACK
			case 0xFE : th->send(th->last_send); return; // NACK
			case 0x00 : // these are errors of some kind
			case 0xFC :
			case 0xFD :
			case 0xFF :
			{	if (th->error) printf("Fatal keyboard error, keyboard now disfunct.\nPress any key, go ahead, it won't do anything now.");
				else
				{	printf("Keyboard error, keyboard will try to re-init");
					th->error = 1;
					asm ("mov $0x20,%%al\n\tout %%al,$0x20\n\t" : : : "%al"); // ack the irq to the PIC
					th->init();
				}
			} break;
			default : if (th->tl) th->tl->feed_scancode(incode);
		}
	}

	void KBC::set_leds(byte status)
	{	send(0xED);
		send(status);
	}

	void KBC::set_translator(Key_Translate *new_tl)
	{	tl = new_tl;
	}

	void KBC::set_keyset(int set)
	{	if (set == 0)
		{	// translate ON
			wait_write();
			outportb(0x64, 0x60);
			wait_write();
			outportb(0x60, 0x65);
		}
		else
		{	// set scanset
			send(0xF0);
			send(set);
			// translate OFF
			wait_write();
			outportb(0x64, 0x60);
			wait_write();
			outportb(0x60, 0x25);
		}
	}

	void KBC::reset_cpu()
	{	byte outputport;
		wait_write();
		outportb(0x64, 0xD0);
		wait_read();
		outputport = inportb(0x60) & 0xFE;
		wait_write();
		outportb(0x64, 0xD1);
		wait_write();
		outportb(0x60, outputport);
		asm("cli");
		asm("hlt");
	}
}
