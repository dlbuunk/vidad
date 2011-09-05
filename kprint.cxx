#include "kernel.h"

static int level = -1;
static void (*ext_puts)(char *);

#define NUM_EARLY 16
#define SIZE_EARLY 64

static char early_msg[NUM_EARLY][SIZE_EARLY];
static int early_num;

static struct msg * cur_msg;
static struct msg * first_msg;

struct msg
{
	msg(void * p)
	{
		// first time constructor, p is a char * in disguise
		char * s = (char *) p;
		int i = 0;
		while (s[i++]);
		str = new char [i];
		for (int j=0; j<i; j++)
			str[j] = s[j];
		time = ktime;
		cur_msg = this;
		first_msg = this;
		next = 0;
	}
	msg(char * s)
	{
		int i = 0;
		while (s[i++]);
		str = new char [i];
		for (int j=0; j<i; j++)
			str[j] = s[j];
		time = ktime;
		cur_msg->next = this;
		cur_msg = this;
		next = 0;
	}
	~msg()
	{
		delete[] str;
	}
	struct msg * next;
	dword time;
	char * str;
};

// it might be needed to call klog_init() several times with different arguments just to get everything right.
void klog_init(void (*puts)(char *))
{
	switch (level)
	{
		default :
			level = 0;
		case 0 :
			early_num = 0;
		case 1 :
			ext_puts = puts;
			break;
		case 2 :
		case 3 :
			ext_puts = puts;
			level = 1;
			break;
	}
	kprint("Kernel logger running, level = %u.", level);
}

void klog_init()
{
	if (level == 0)
	{
		level = 1;

		// first message
		char temp[80];
		for (int i=0; i<78; i++)
			temp[i] = '=';
		temp[78] = '\n';
		temp[79] = '\0';
		new msg((void *) temp);

		// copy messages
		for (int i=0; i<early_num; i++)
			new msg(early_msg[i]);
	}
	kprint("Kernel logger running, level = %u.", level);
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

	// copy (and format) string
	i = 16;
	dword * ptr; // dword *, not void *, as ISO C++ forbids incrementing a pointer of type 'void*'
	ptr = (dword *) &istr;
	dword unum;
	char * str;
	while (*istr)
	{
		if (*istr != '%')
			ostr[i++] = *istr++;

		else switch (*++istr)
		{
			case '%' :
				ostr[i++] = *istr++;
				break;

			case 'X' :
				unum = *++ptr;
				if ((unum & 0xF0000000) > 0x90000000)
					ostr[i++] = (unum >> 28) + 0x37;
				else
					ostr[i++] = (unum >> 28) + 0x30;
				if ((unum & 0xF000000) > 0x9000000)
					ostr[i++] = (unum >> 24 & 0xF) + 0x37;
				else
					ostr[i++] = (unum >> 24 & 0xF) + 0x30;
				if ((unum & 0xF00000) > 0x900000)
					ostr[i++] = (unum >> 20 & 0xF) + 0x37;
				else
					ostr[i++] = (unum >> 20 & 0xF) + 0x30;
				if ((unum & 0xF0000) > 0x90000)
					ostr[i++] = (unum >> 16 & 0xF) + 0x37;
				else
					ostr[i++] = (unum >> 16 & 0xF) + 0x30;
				if ((unum & 0xF000) > 0x9000)
					ostr[i++] = (unum >> 12 & 0xF) + 0x37;
				else
					ostr[i++] = (unum >> 12 & 0xF) + 0x30;
				if ((unum & 0xF00) > 0x900)
					ostr[i++] = (unum >> 8 & 0xF) + 0x37;
				else
					ostr[i++] = (unum >> 8 & 0xF) + 0x30;
				if ((unum & 0xF0) > 0x90)
					ostr[i++] = (unum >> 4 & 0xF) + 0x37;
				else
					ostr[i++] = (unum >> 4 & 0xF) + 0x30;
				if ((unum & 0xF) > 0x9)
					ostr[i++] = (unum & 0xF) + 0x37;
				else
					ostr[i++] = (unum & 0xF) + 0x30;
				istr++;
				break;

			case 'x' :
				unum = *++ptr;
				if (unum > 0xF)
				{
					if (unum > 0xF0)
					{
						if (unum > 0xF00)
						{
							if (unum > 0xF000)
							{
								if (unum > 0xF0000)
								{
									if (unum > 0xF00000)
									{
										if (unum > 0xF000000)
										{
											if ((unum & 0xF0000000) > 0x90000000)
												ostr[i++] = (unum >> 28) + 0x37;
											else
												ostr[i++] = (unum >> 28) + 0x30;
										}
										if ((unum & 0xF000000) > 0x9000000)
											ostr[i++] = (unum >> 24 & 0xF) + 0x37;
										else
											ostr[i++] = (unum >> 24 & 0xF) + 0x30;
									}
									if ((unum & 0xF00000) > 0x900000)
										ostr[i++] = (unum >> 20 & 0xF) + 0x37;
									else
										ostr[i++] = (unum >> 20 & 0xF) + 0x30;
								}
								if ((unum & 0xF0000) > 0x90000)
									ostr[i++] = (unum >> 16 & 0xF) + 0x37;
								else
									ostr[i++] = (unum >> 16 & 0xF) + 0x30;
							}
							if ((unum & 0xF000) > 0x9000)
								ostr[i++] = (unum >> 12 & 0xF) + 0x37;
							else
								ostr[i++] = (unum >> 12 & 0xF) + 0x30;
						}
						if ((unum & 0xF00) > 0x900)
							ostr[i++] = (unum >> 8 & 0xF) + 0x37;
						else
							ostr[i++] = (unum >> 8 & 0xF) + 0x30;
					}
					if ((unum & 0xF0) > 0x90)
						ostr[i++] = (unum >> 4 & 0xF) + 0x37;
					else
						ostr[i++] = (unum >> 4 & 0xF) + 0x30;
				}
				if ((unum & 0xF) > 0x9)
					ostr[i++] = (unum & 0xF) + 0x37;
				else
					ostr[i++] = (unum & 0xF) + 0x30;
				istr++;
				break;

			case 'u' :
				unum = *++ptr;
				if (unum >= 10)
				{
					if (unum >= 100)
					{
						if (unum >= 1000)
						{
							if (unum >= 10000)
							{
								if (unum >= 100000)
								{
									if (unum >= 1000000)
									{
										if (unum >= 10000000)
										{
											if (unum >= 100000000)
											{
												if (unum >= 1000000000)
												{
													ostr[i++] = unum / 1000000000 + 0x30;
													unum %= 1000000000;
												}
												ostr[i++] = unum / 100000000 + 0x30;
												unum %= 100000000;
											}
											ostr[i++] = unum / 10000000 + 0x30;
											unum %= 10000000;
										}
										ostr[i++] = unum / 1000000 + 0x30;
										unum %= 1000000;
									}
									ostr[i++] = unum / 100000 + 0x30;
									unum %= 100000;
								}
								ostr[i++] = unum / 10000 + 0x30;
								unum %= 10000;
							}
							ostr[i++] = unum / 1000 + 0x30;
							unum %= 1000;
						}
						ostr[i++] = unum / 100 + 0x30;
						unum %= 100;
					}
					ostr[i++] = unum / 10 + 0x30;
					unum %= 10;
				}
				ostr[i++] = unum + 0x30;
				istr++;
				break;

			case 'o' :
				unum = *++ptr;
				if (unum > 07)
				{
					if (unum > 077)
					{
						if (unum > 0777)
						{
							if (unum > 07777)
							{
								if (unum > 077777)
								{
									if (unum > 0777777)
									{
										if (unum > 07777777)
										{
											if (unum > 077777777)
											{
												if (unum > 0777777777)
												{
													if (unum > 07777777777)
													{
														ostr[i++] = (unum >> 30) + 0x30;
														unum &= 07777777777;
													}
													ostr[i++] = (unum >> 27) + 0x30;
													unum &= 0777777777;
												}
												ostr[i++] = (unum >> 24) + 0x30;
												unum &= 077777777;
											}
											ostr[i++] = (unum >> 21) + 0x30;
											unum &= 07777777;
										}
										ostr[i++] = (unum >> 18) + 0x30;
										unum &= 0777777;
									}
									ostr[i++] = (unum >> 15) + 0x30;
									unum &= 077777;
								}
								ostr[i++] = (unum >> 12) + 0x30;
								unum &= 07777;
							}
							ostr[i++] = (unum >> 9) + 0x30;
							unum &= 0777;
						}
						ostr[i++] = (unum >> 6) + 0x30;
						unum &= 077;
					}
					ostr[i++] = (unum >> 3) + 0x30;
					unum &= 07;
				}
				ostr[i++] = unum + 0x30;
				istr++;
				break;

			case 's' :
				str = (char *) *++ptr;
				while (*str)
					ostr[i++] = *str++;
				istr++;
				break;

			default :
				;
		}
	}


	// instert trailing newline 
	ostr[i++] = '\n';
	ostr[i] = '\0';

	// print it!
	switch (level)
	{
		case 0 :
			if (early_num >= NUM_EARLY) break;
			for (i=0; i<SIZE_EARLY; i++)
				early_msg[early_num][i] = ostr[i];
			early_num++;
			(*ext_puts)(ostr);
			break;

		case 1 :
			new msg(ostr);
			(*ext_puts)(ostr);
			break;

		default :
			;
	}
}

