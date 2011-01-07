#include <kernel.h>

word kprint_pos = 0;

void kprint(char ch, byte color)
{	if (ch == '\n')
	{	kprint_pos = (kprint_pos/80 + 1) * 80;
		goto end;
	}
	if (ch == '\t')
	{	kprint_pos = ((kprint_pos>>3)+1) << 3;
		goto end;
	}
	*((word *) (0xB8000+(kprint_pos<<1))) = (color<<8) | ch;
	kprint_pos++;
	end: ;
	outportb(0x03D4, 0x0F);
	outportb(0x03D5, (byte) kprint_pos);
	outportb(0x03D4, 0x0E);
	outportb(0x03D5, (byte) (kprint_pos>>8));
}

void kprints(char *str, byte color)
{	do kprint(*str, color); while (*++str);
}
