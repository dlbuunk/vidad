/* loader.c, in the end, the goal is to call this beast:
void (*kernel)(unsigned int (*bb_read_block)(unsigned char *ptr, unsigned int block), void (*bb_exit_hw)(void), struct Memdata * memdata, struct Info * info, void * IDTP, void * GDTP)
*/

typedef unsigned char byte;
typedef unsigned short int word;

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

void lmain(void)
{
	screen_init();
	screen_puts("Loading VIOS...\n");
}

