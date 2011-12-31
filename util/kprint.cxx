//      kprint.cxx
//      
//      Copyright 2011 D.L.Buunk <dlbuunk@gmail.com>
//      
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//      
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//      
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
//      MA 02110-1301, USA.

#include "util.hxx"

namespace util
{

void (*_loader_puts)(char const *);

void kputs(char const * str)
{
	char buff[178] = { "[   0:00:00.00] " } ;
	strncat(buff, str, 160);
	strcat(buff, "\n");
	_kprints(buff);
}

void _kprints(char const * str)
{
	// For now, only use the loader_puts, later this should be inproved.
	(*_loader_puts)(str);
}

// These functions are far from 'safe', so care must be taken when using them.
void _format_str(char const * fstr, char * ostr, dword * args)
{
	while (*fstr != '\0')
	{
		if (*fstr == '%') switch (*++fstr)
		{
		case 't': // ViOS kernel specific, needs inprovement.
			strcpy(ostr, "[   0:00:00.00]");
			ostr += strlen(ostr);
			break;
		case 'c':
			*ostr++ = (char const)(*args++);
			break;
		case 's':
			strcpy(ostr, (char const *)(*args++));
			ostr += strlen(ostr);
			break;
		case 'x':
		case 'X':
			for (int i=28; i>=0; i-=4)
			{
				*ostr = ((*args >> i) & 0xF) + 0x30;
				if (*ostr > 0x39)
					*ostr += *fstr - 'X' + 7;
				ostr++;
			}
			args++;
			break;
		case 'u':
			break;
		case 'i':
		case 'd':
			break;
		default:
			*ostr++ = *fstr;
		}
		else
			*ostr++ = *fstr;
		fstr++;
	}
	*ostr = '\0';
}

int kprintf(char const * fstr, ...)
{
	char ostr[512];
	_format_str(fstr, ostr, (dword *) &fstr + 1);
	_kprints(ostr);
	return strlen(ostr);
}

int sprintf(char * ostr, char const * fstr, ...)
{
	_format_str(fstr, ostr, (dword *) &fstr + 1);
	return strlen(ostr);
}

}
