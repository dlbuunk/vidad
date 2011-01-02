// this line should be changed in the future
#include "io/io.h"

// kernel terminal, this pointer is needed by ccerror()
IO::Terminal *kterm;

// overload new and delete
void *operator new(dword size)
{	return calloc(1, size);
};
void *operator new[](dword size)
{	return calloc(1, size);
};
void operator delete(void *p)
{	return free(p);
};
void operator delete[](void *p)
{	return free(p);
};

// back up function for pure virtual functions
extern "C" void __cxa_pure_virtual()
{	kerror("Fatal error: cannot make pure virtual function call\n", WHITE_GREEN);
};

// kernel entry function
extern "C" void ccentry(struct kheader *kh, dword dma_buff)
{	(void) kh;
	(void) dma_buff;
	kterm = 0; // do this in order to make things like 'if (kterm)' work
	IO::Init *init = new IO::Init;
	init->main();
	delete init;
};

// function to print out fatal error messages
// TODO: make compatible with kernel terminal
extern "C" void kerror(char *str, byte color)
{	memstow(0xB8000, 2000, 0x0020 | (color<<8));
	kprint_pos = 0;
	kprints(str, color);
	kprints("\nSYSTEM HALTED", color);
	asm("cli");
	asm("hlt");
};

// basic (s)printf functions
// currently it only supports strings, hex and unsigned decimal
// width supported, no other optional stuff.
char print_buffer[0x100];

void print(char *str, char **form)
{	char *format;
	format = *form;
	dword *varg;
	varg = (dword *) form;
	varg++;
	byte num[10];
	int num_count;
	int min_width, width;
	while (*format)
	{	if (*format == '%')
		{	format++;
			width = 0;
			min_width = 1;
			if (*format == '\0') break;
			if (*format == '%') goto out;
			if (*format <= '9' && *format >= '0')
			{	min_width = *format - 0x30;
				format++;
			}
			if (*format == '\0') break;
			switch (*format)
			{	case 's' : //string
				{	while (**((char **) varg))
					{	*str = **((char **) varg);
						str++;
						(*varg)++;
					}
					format++;
					varg++;
				} break;
				case 'X' : //uppercase hex
				{	for (num_count=0; num_count<10; num_count++)
					{	num[num_count] = (*varg) & 0xF;
						(*varg) >>= 4;
						if (num[num_count]) width = num_count + 1;
					}
					if (min_width > width) width = min_width;
					for (num_count = width - 1; num_count >= 0; num_count--)
					{	num[num_count] += 0x30;
						if (num[num_count] > 0x39) num[num_count] += 7;
						*str = num[num_count];
						str++;
					}
					format++;
					varg++;
				} break;
				case 'u' : //unsigned int
				{	for (num_count=0; num_count<10; num_count++)
					{	num[num_count] = (*varg) % 10;
						(*varg) /= 10;
						if (num[num_count]) width = num_count + 1;
					}
					if (min_width > width) width = min_width;
					for (num_count = width - 1; num_count >= 0; num_count--)
					{	num[num_count] += 0x30;
						*str = num[num_count];
						str++;
					}
					format++;
					varg++;
				} break;
			}
		}
		else
		{	out: *str = *format;
			str++;
			format++;
		}
	}
	*str = '\0';
};

void sprintf(char *str, char *format, ...)
{	print(str, &format);
};

void printf(char *format, ...)
{	print(print_buffer, &format);
	if (kterm) kterm->puts(print_buffer);
	else kprints(print_buffer, W);
};
