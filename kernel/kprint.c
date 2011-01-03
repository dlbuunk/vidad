#include "kernel.h"

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
};

void kprints(char *str, byte color)
{	do kprint(*str, color); while (*++str);
};

void kprintb(byte val, byte color)
{	byte out;
	out = val;
	val >>= 4;
	val += 0x30;
	if (val > 0x39) val += 7;
	kprint(val, color);
	out &= 0x0F;
	out += 0x30;
	if (out > 0x39) out += 7;
	kprint(out, color);
};

void kprintw(word val, byte color)
{	word out;
	out = val;
	val >>= 8;
	kprintb(val, color);
	kprintb(out, color);
};

void kprintd(dword val, byte color)
{	dword out;
	out = val;
	val >>= 16;
	kprintw(val, color);
	kprintw(out, color);
};
