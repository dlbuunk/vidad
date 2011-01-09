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

namespace IO
{	Term_Buf::Term_Buf(Video_Text *video, byte col, byte row)
	{	buffer = new word [col*(row+1)];
		memstow((word *) buffer, 0x0720, col*(row+1));
		vid = video;
		cursor_pos = 0;
		num_col = col;
		num_row = row;
	}

	Term_Buf::~Term_Buf()
	{	delete buffer;
	}
}
