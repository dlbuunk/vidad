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

void kprints(const char *str, byte color)
{	do kprint(*str, color); while (*++str);
}
