#include "../io.h"

extern IO::Terminal *kterm;

// TODO:
// create gets()
// make the entire thing resizeable

namespace IO
{	Term_Std::Term_Std(Term_Buf *stderr, Term_Buf *stdout)
	{	err = stderr;
		out = stdout;
		current_color = W;
	};

	Term_Std::~Term_Std() {};

	void Term_Std::puts_err(char *str)
	{	if (err == 0) return;
		while (*str)
		{	switch (*str)
			{	case '\n' : err->cursor_pos = (err->cursor_pos/err->num_col + 1) * err->num_col; break;
				case '\c' : err->cursor_pos = (err->cursor_pos/err->num_col) * err->num_col; break;
				case '\t' : err->cursor_pos = ((err->cursor_pos>>3) + 1) << 3; break;
				default :
				{	err->buffer[err->cursor_pos] = (current_color<<8) | *str;
					err->cursor_pos++;
				}
			}
			str++;
			if (err->cursor_pos >= err->num_col*err->num_row)
			{	memcpyw((dword) err->buffer, (err->num_row) * err->num_col , (dword) err->buffer + (err->num_col<<1));
				err->cursor_pos -= err->num_col;
			}
		}
		err->vid->redraw(err->buffer, err->cursor_pos);
	};

	void Term_Std::puts(char *str)
	{	if (out == 0) return;
		while (*str)
		{	switch (*str)
			{	case '\n' : out->cursor_pos = (out->cursor_pos/out->num_col + 1) * out->num_col; break;
				case '\c' : out->cursor_pos = (out->cursor_pos/out->num_col) * out->num_col; break;
				case '\t' : out->cursor_pos = ((out->cursor_pos>>3) + 1) << 3; break;
				default :
				{	out->buffer[out->cursor_pos] = (current_color<<8) | *str;
					out->cursor_pos++;
				}
			}
			str++;
			if (out->cursor_pos >= out->num_col*out->num_row)
			{	memcpyw((dword) out->buffer, (out->num_row) * out->num_col , (dword) out->buffer + (out->num_col<<1));
				out->cursor_pos -= out->num_col;
			}
		}
		out->vid->redraw(out->buffer, out->cursor_pos);
	};

	char *Term_Std::gets(char *str) { return(str); };

	void Term_Std::set_active()
	{	out->vid->current_buffer = out->buffer;
		out->vid->redraw(out->buffer, out->cursor_pos);
	};

	void Term_Std::set_color(byte color)
	{	current_color = color;
	};
};
