/* standard typedefs */
typedef unsigned char byte;
typedef unsigned short int word;
typedef unsigned int dword;

/* inb and outb */
#define outb(port, data) asm ( \
	"outb %%al,%%dx\n\t" \
	: /* no outputs */ \
	: "a" (data) \
	, "d" (port) \
	)

byte inb(word port)
{
	byte data;
	asm (
	"inb %%dx,%%al\n\t"
	: "=a" (data)
	: "d" (port)
	) ;
	return(data);
}

/* screen output stuff */
word screen_pos;

void screen_init()
{
	int i;
	outb(0x03C2, inb(0x03CC) | 0x01);
	screen_pos = 0;
	for (i=0; i<2000; i++)
		*((word *) (0x000B8000 + (i<<1))) = 0x0720;
	outb(0x03D4, 0x0F);
	outb(0x03D5, (byte)(screen_pos&0xFF));
	outb(0x03D4, 0x0E);
	outb(0x03D5, (byte)((screen_pos>>8)&0xFF));
}

void screen_putchar(char ch)
{
	int i;
	if (ch == '\n') screen_pos = (screen_pos/80+1)*80;
	else *((word *) (0x000B8000 + (screen_pos++<<1))) = 0x0700 | (word) ch;
	if (screen_pos >= 2000)
	{
		for (i=0; i<2000; i++)
			*((word *) (0x000B8000 + (i<<1))) = *((word *) (0x000B8000 + (i<<1) + 160));
		screen_pos -= 80;
	}
	outb(0x03D4, 0x0E);
	outb(0x03D5, screen_pos>>8);
	outb(0x03D4, 0x0F);
	outb(0x03D5, screen_pos);
}

void screen_puts(char *str)
{
	while (*str) screen_putchar(*str++);
}

/* all bb_ functions, for handling the floppy subsystem */
extern void bb_timer(dword);
extern void bb_waitf(void);
extern byte bb_readF(void);
extern void bb_writeF(byte);
extern void bb_exit(void);
byte bb_cur_track = -1;

int bb_load_raw(byte track)
{
	int i;

	/* seek first */
	for (i=0; i<10; i++)
	{
		bb_writeF(0x0F);
		bb_writeF(0x00);
		bb_writeF(track);
		bb_waitf();
		bb_writeF(0x08);
		(void) bb_readF();
		if (bb_readF() == track) goto dma;
	}
	return(-1);

	dma: /* setup DMA */
	outb(0x0A, 0x06);
	outb(0x0C, 0xFF);
	outb(0x04, 0x00);
	outb(0x04, 0x80);
	outb(0x81, 0x00);
	outb(0x0C, 0xFF);
	outb(0x05, 0xFF);
	outb(0x05, 0x47);
	outb(0x0B, 0x46);
	outb(0x0A, 0x02);

	/* wait a while */
	bb_timer(3);

	/* read command to FIFO */
	bb_writeF(0xC6);
	bb_writeF(0x00);
	bb_writeF(track);
	bb_writeF(0x00);
	bb_writeF(0x01);
	bb_writeF(0x02);
	bb_writeF(0x12);
	bb_writeF(0x1B);
	bb_writeF(0xFF);

	bb_waitf();

	for (i=0; i<7; i++)
		(void) bb_readF();
	/* ToDo: error-checks on status values */

	bb_cur_track = track;
	return(0);
}

int bb_read_block(byte *buffer, int bnum)
{
	byte track;
	int rt, i;

	if (bnum < 0 || bnum > 360)
		return(1);

	track = (bnum<<1) / 9;
	if (track != bb_cur_track && (rt = bb_load_raw(track)))
		return(rt);

	if ((bnum % 9) == 4) /* block split over 2 tracks */
	{
		for (i=0; i<0x0800; i++)
			buffer[i] = *((byte *) (0x0000C000 + i));
		if (rt = bb_load_raw(++track))
			return(rt);
		for (i=0; i<0x0800; i++)
			buffer[i] = *((byte *) (0x00008000 + i));
	}
	else for (i=0; i<0x1000; i++)
		buffer[i] = *((byte *) (0x00008000 + (((bnum<<1)%9)<<11) + i));
	return(0);
}

/* lmain function */
void lmain(void)
{
	word block;
	byte * kptr = (byte *) 0x0000F000; /* 0x1000 is added later */
	screen_init();
	screen_puts("Loading VIOS...\n");

	/* find initial block */
	block = *((word *) 0x1212);

	do /* load blocks */
	{
		if (block <= 0 || block > 360)
		{
			screen_puts("Error finding kernel");
			return;
		}
		if(bb_read_block(kptr+=0x1000, block))
		{
			screen_puts("Error loading kernel");
			return;
		}
		block = *(((word *) (*((word *) 0x120C) + 0x1000)) + block);
	} while (block != 0xFFF8);

	screen_puts("Kernel loaded.\n");

	/* now call the kernel (this is going to be funny) */
	(**((void (**)(int (*)(byte *, int), void (*)(char *), void (*)(dword), void (*)(void), dword, dword, dword, dword)) 0x00010000))(&bb_read_block, &screen_puts, &bb_timer, &bb_exit, 0x0600, 0x07F4, 0xFF02, 0xFF0A);
}

