/* standard typedefs */
typedef unsigned char byte;
typedef unsigned short int word;
typedef unsigned int dword;
typedef unsigned long long int qword;

/* output functions */
extern void (*bb_puts)(char *);

void hexb(byte v)
{
	char t[5];
	t[0] = '0';
	t[1] = 'x';
	if ((t[2] = (v>>4) + 0x30) > 0x39) t[2] += 0x07;
	if ((t[3] = (v&15) + 0x30) > 0x39) t[3] += 0x07;
	t[4] = 0;
	(*bb_puts)(t);
}

void hexw(word v)
{
	char t[3];
	hexb(v>>8);
	if ((t[0] = ((v&0xF0)>>4) + 0x30) > 0x39) t[0] += 0x07;
	if ((t[1] = (v&0xF) + 0x30) > 0x39) t[1] += 0x07;
	t[2] = 0;
	(*bb_puts)(t);
}

void hexd(dword v)
{
	char t[5];
	hexw(v>>16);
	if ((t[0] = ((v>>12)&0xF) + 0x30) > 0x39) t[0] += 0x07;
	if ((t[1] = ((v>>8)&0xF) + 0x30) > 0x39) t[1] += 0x07;
	if ((t[2] = ((v>>4)&0xF) + 0x30) > 0x39) t[2] += 0x07;
	if ((t[3] = (v&0xF) + 0x30) > 0x39) t[3] += 0x07;
	t[4] = 0;
	(*bb_puts)(t);
}

/* memory functions */
void memstob(byte *addr, byte val, dword count)
{
	asm (
		"\tcld\n"
		"\trep\tstosb\n"
		: /* no outputs */
		: "a" (val)
		, "c" (count)
		, "D" (addr)
	) ;
}
