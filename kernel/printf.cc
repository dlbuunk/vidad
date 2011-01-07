#include <kernel.h>
#include <io/io.h>

// basic (s)printf functions
// currently it only supports chars, strings, hex and unsigned decimal
// width supported, no other optional stuff.
char print_buffer[0x100];

void print(char *str, char **form)
{	char *str_orig;
	str_orig = str;
	char *format;
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
			{	case 'c' : //char
				{	if (*((char *) varg) == '\b')
					{	if (str != str_orig) str--;
						*str = '\0';
					}
					else
					{	*str = *((char *) varg);
						str++;
					}
					format++;
					varg++;
				} break;
				case 's' : //string
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
		else if (*format == '\b')
		{	if (str != str_orig) str--;
			*str = '\0';
			format++;
		}
		else
		{	out: *str = *format;
			str++;
			format++;
		}
	}
	*str = '\0';
}

void sprintf(char *str, char *format, ...)
{	print(str, &format);
}

void printf(char *format, ...)
{	print(print_buffer, &format);
	if (kterm) kterm->puts(print_buffer);
	else kprints(print_buffer, 0x07);
}
