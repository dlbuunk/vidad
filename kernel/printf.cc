/* This file is part of Vidad.
*
*  Vidad is free software: you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  Vidad is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with Vidad.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <kernel.h>
#include <io/io.h>

// basic (s)printf functions
// currently it only supports chars, strings, hex and unsigned decimal
// width supported, no other optional stuff.
char print_buffer[0x100];

// As far as I can tell, this function takes a target string and a pointer
// to the format args (the first of which is a string of the format, and
// the next are parameters to be added to the format.
// Unclear: Why is form not a char* const* ?
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
