//      loader_hw.c
//      
//      Copyright 2011 D.L.Buunk <dlbuunk@gmail.com>
//      
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//      
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//      
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
//      MA 02110-1301, USA.

// General typedefs
typedef unsigned char byte;
typedef unsigned short int word;
typedef unsigned int dword;

// Common inb/outb functions
void outb(byte val, word port)
{
	asm volatile (
		"outb	%0,%1\n\t"
		:
		: "a" (val)
		, "Nd" (port)
		:
	);
}

byte inb(word port)
{
	byte val;
	asm volatile (
		"inb	%1,%0\n\t"
		: "=a" (val)
		: "Nd" (port)
		:
	);
	return val;
}

// Screen functions
void putchar(char ch)
{
	// Get cursor position from vga registers.
	word cursor_pos;
	outb(0x0E, 0x03D4);
	cursor_pos = inb(0x03D5) << 8;
	outb(0x0F, 0x03D4);
	cursor_pos |= inb(0x03D5);

	if (ch == '\n')
		cursor_pos = (cursor_pos/80 + 1) * 80;
	else if (ch == '\t')
		cursor_pos = ((cursor_pos>>3) + 1) << 3;
	else
		*((word *)(0x000B8000 + (cursor_pos++ <<1))) = 0x0700 | ch;

	if (cursor_pos >= 2000)
	{
		for (int i=0; i<4000; i+=2)
			*((word *)(0x000B8000 + i)) =
				*((word *)(0x000B80A0 + i));
		cursor_pos -= 0x50;
	}

	// Set cursor position.
	outb(0x0E, 0x03D4);
	outb(cursor_pos>>8, 0x03D5);
	outb(0x0F, 0x03D4);
	outb(cursor_pos, 0x03D5);
}

void puts(char *str)
{
	while (*str)
	{
		putchar(*str);
		str++;
	}
}

// Irq handlers && functions
extern void irq0(void);
extern void irq6(void);
extern void wait_irq0(void);
extern void wait_irq6(void);

// FDC helper functions

void timer(int ticks)
{
	for (;ticks>=0;ticks--)
		wait_irq0();
}

byte read_fifo(void)
{
	while (! (inb(0x03F4) & 0x80));
	return inb(0x03F5);
}

void write_fifo(byte val)
{
	while (! (inb(0x03F4) & 0x80));
	outb(val, 0x03F5);
}

word sense_int(void)
{
	word status;
	write_fifo(0x08);
	status = read_fifo() << 8;
	status |= read_fifo();
	return status;
}

// FDC read function
void floppy_read(void * addr, byte track, byte head)
{
	// FDC seek.
	do
	{
		write_fifo(0x0F);
		write_fifo(0x00);
		write_fifo(track);
		wait_irq6();
	} while ((sense_int() & 0x00FF) != track);

	// Set up DMA.
	outb(0xFF, 0x0C);
	outb(0xFF, 0x05);
	outb(0x23, 0x05);
	outb(0xFF, 0x0C);
	outb((byte)((dword) addr), 0x04);
	outb((byte)(((dword) addr)>>8), 0x04);
	outb((byte)(((dword) addr)>>16), 0x81);
	outb(0x64, 0x0B);
	outb(0x0B, 0x0F);

	// FDC read.
	write_fifo(0x46);
	write_fifo(head<<2);
	write_fifo(track);
	write_fifo(head);
	write_fifo(0x01);
	write_fifo(0x02);
	write_fifo(0x12);
	write_fifo(0x1B);
	write_fifo(0xFF);
	wait_irq6();
	for (int i=0; i<7; i++)
		(void) read_fifo();
}

// function to read a block (4kb) for use by loader_main()
word block_loaded[3] = {0xFFFF, 0xFFFF, 0xFFFF};

void read_block(void * addr, word block)
{
	int bload = -1;
	for (int i=0; i<3; i++)
		if (block == block_loaded[i])
		{
			bload = i;
			goto end;
		}

	byte track, head = 0;
	word mblock;
	track = block/9 * 2;
	mblock = block % 9;
	if (block > 4) track++;
	if (block == 3 || block == 4 || block == 7 || block == 8) head = 1;
	floppy_read((void *) 0x8000, track, head);
	block_loaded[2] = 0xFFFF;

	switch (mblock)
	{
		case 0:
			block_loaded[0] = block;
			block_loaded[1] = block+1;
			bload = 0;
			break;
		case 1:
			block_loaded[0] = block-1;
			block_loaded[1] = block;
			bload = 1;
			break;
		case 2:
			for (int i=0; i<1024; i++)
				*((byte *)(0x8000+i)) = *((byte *)(0xA000+i));
			floppy_read((void *) 0x8400, track, 1);
			block_loaded[0] = block;
			block_loaded[1] = block+1;
			bload = 0;
			break;
		case 3:
			for (int i=0; i<4096; i++)
				*((byte *)(0x8000+i)) = *((byte *)(0x8C00+i));
			block_loaded[0] = block;
			block_loaded[1] = 0xFFFF;
			bload = 0;
			break;
		case 4:
			for (int i=0; i<2048; i++)
				*((byte *)(0x8000+i)) = *((byte *)(0x9C00+i));
			floppy_read((void *) 0x8800, track+1, 0);
			block_loaded[0] = block;
			block_loaded[1] = block+1;
			bload = 0;
		case 5:
			for (int i=0; i<4096; i++)
				*((byte *)(0x8000+i)) = *((byte *)(0x8800+i));
			block_loaded[0] = block;
			block_loaded[1] = 0xFFFF;
			bload = 0;
			break;
		case 6:
			for (int i=0; i<3072; i++)
				*((byte *)(0x8000+i)) = *((byte *)(0x9800+i));
			floppy_read((void *) 0x8C00, track, 1);
			block_loaded[0] = block;
			block_loaded[1] = block+1;
			block_loaded[2] = block+2;
			bload = 0;
			break;
		case 7:
			for (int i=0; i<8192; i++)
				*((byte *)(0x8000+i)) = *((byte *)(0x8400+i));
			block_loaded[0] = block;
			block_loaded[1] = block+1;
			bload = 0;
			break;
		case 8:
			for (int i=0; i<8192; i++)
				*((byte *)(0x8000+i)) = *((byte *)(0x8400+i));
			block_loaded[0] = block-1;
			block_loaded[1] = block;
			bload = 1;
			break;
	}

	end: for (int i=0; i<4096; i++)
		*(((byte *)addr) + i) = *((byte *) 0x8000 + 0x1000 * bload + i);
}

// Hardware main loader
extern void loader_main(void);

void exit_hw(void)
{
	// Floppy motor off.
	outb(0x0C, 0x03F2);
	timer(10); // 100 ms

	// FDC off.
	outb(0x00, 0x03F2);

	// Hardware interrupts off.
	asm ("cli\n\t" : : : );

	// Turn DMAC off.
	outb(0x0F, 0x0F);
	outb(0x0F, 0xDE);
	outb(0x04, 0x08);
	outb(0x04, 0xD0);

	// Mask all IRQs.
	outb(0xFF, 0x21);
}

void loader_hw(void)
{
	// Tell the (l)user what we are up to.
	puts("OK\nInit hw ");

	// setup interrupt vectors, we know that the high part remains 0x0000
	*((word *) 0x00000900) = (word)((dword) &irq0);
	*((word *) 0x00000930) = (word)((dword) &irq6);

	// Initialise the PIC.
	outb(0x11, 0x20);
	outb(0x20, 0x21);
	outb(0x04, 0x21);
	outb(0x01, 0x21);
	outb(0xBE, 0x21);

	// Initialise the PIT at 100 Hz.
	outb(0x34, 0x43);
	outb(0x9C, 0x40);
	outb(0x2E, 0x40);

	// Initialise the DMAC.
	outb(0x00, 0xD0); // turn on
	outb(0x00, 0x08);
	outb(0xFF, 0xDA); // reset
	outb(0xFF, 0x0D);
	outb(0x0E, 0xDE); // masks
	outb(0x0F, 0x0F);

	// Hardware interrupts on.
	asm ("sti\n\t" : : : );

	// Initialise the FDC.
	outb(0x0C, 0x03F2);
	wait_irq6();

	(void) sense_int();

	// CCR
	outb(0x00, 0x03F7);

	// configure
	write_fifo(0x13);
	write_fifo(0x00);
	write_fifo(0x47);
	write_fifo(0x00);

	// specify
	write_fifo(0x03);
	write_fifo(0xC0);
	write_fifo(0x10);

	// motor on
	outb(0x1C, 0x03F2);
	timer(30);	// 300 ms

	// calibrate
	do
	{
		write_fifo(0x07);
		write_fifo(0x00);
		wait_irq6();
	} while (sense_int() & 0x00FF);

	puts("OK\n");

	// Call the main loader.
	(void) &exit_hw;
	loader_main();

	exit_hw();
}
