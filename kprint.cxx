#include "kernel.h"

static int level = -1;
static void (*ext_puts)(char *);

void klog_init(int new_level, void (*puts)(char *))
{
	ext_puts = puts;
	level = new_level;
}

void kprint(char const * istr, ...)
{
	char ostr[256];		// ought to be enough for everyone
	dword time;
	int i;

	// insert timestamp
	ostr[0] = '[';
	ostr[1] = ostr[2] = ostr[3] = ' ';
	time = (ktime>>8) / 3600;
	ostr[4] = (time % 10) + 0x30;
	if ((time /= 10))
	{
		ostr[3] = (time % 10) + 0x30;
		if ((time /= 10))
		{
			ostr[2] = (time %10) + 0x30;
			if ((time /= 10))
				ostr[1] = time + 0x30;
		}
	}
	ostr[5] = ':';
	time = ((ktime>>8) / 60) % 60;
	ostr[6] = (time / 10) + 0x30;
	ostr[7] = (time % 10) + 0x30;
	ostr[8] = ':';
	time = (ktime>>8) % 60;
	ostr[9] = (time / 10) + 0x30;
	ostr[10] = (time % 10) + 0x30;
	ostr[11] = '.';
	time = ktime & 0xFF;
	ostr[12] = (time >> 4) + 0x30;
	if (ostr[12] > 0x39) ostr[12] += 7;
	ostr[13] = (time & 0xF) + 0x30;
	if (ostr[13] > 0x39) ostr[13] += 7;
	ostr[14] = ']';
	ostr[15] = ' ';

	// copy string
	i = 16;
	while (*istr)
		ostr[i++] = *istr++;
	// instert trailing newline 
	ostr[i++] = '\n';
	ostr[i] = '\0';

	// print it!
	switch (level)
	{
		case 0 :
			(*ext_puts)(ostr);
			break;
		default :
			break;
	}
}

