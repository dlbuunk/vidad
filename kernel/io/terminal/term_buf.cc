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
