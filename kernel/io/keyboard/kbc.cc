#include "../io.h"

// port 0x64 is command/status
// port 0x60 is data
namespace IO
{	KBC::KBC(ByteBuffer *outbuffer)
	{	byte result;
		ob = outbuffer;

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

		// get irq handle
		if ((inter_num = inter_reg((dword) &handle_irq, (dword) this, 0x01)) == -1)
		{	printf("Keyboard, KBC: error, cannot get irq1 handle, keyboard won't function.\n");
		}
	};

	KBC::~KBC()
	{	// remove irq handle
		inter_dereg(0x01, inter_num);
		// disable keyboard
		wait_write();
		outportb(0x64, 0xAD);
		printf("Keyboard, KBC: keyboard disabled.\n");
	};

	void KBC::wait_read()
	{	while (! (inportb(0x64) & 0x01));
	};

	void KBC::wait_write()
	{	while (inportb(0x64) & 0x02);
	};

	void KBC::handle_irq(dword ptr)
	{	KBC *th;
		th = (KBC *) ptr;
		th->wait_read();
		if (th->ob->write(inportb(0x60))) printf("Keyboard, KBC: output buffer full.\n");
	};

	void KBC::set_leds(byte status)
	{	wait_write();
		outportb(0x60, 0xED);
		wait_write();
		outportb(0x60, status);
	};

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
	};
};
