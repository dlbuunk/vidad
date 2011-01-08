#include <kernel.h>
#include <io/io.h>

extern IO::Terminal *kterm;

// TODO:
// add support for arrow keys
// make the entire thing resizeable

namespace IO
{	Term_Std::Term_Std(Term_Buf *stderr, Term_Buf *stdout, WordBuffer *stdin)
	{	err = stderr;
		out = stdout;
		in = stdin;
		current_color = 0x07;
	}

	Term_Std::~Term_Std() {}

	void Term_Std::puts_err(char *str)
	{	if (err == 0) return;
		while (*str)
		{	switch (*str)
			{	case '\n' : err->cursor_pos = (err->cursor_pos/err->num_col + 1) * err->num_col; break;
				case '\r' : err->cursor_pos = (err->cursor_pos/err->num_col) * err->num_col; break;
				case '\t' : err->cursor_pos = ((err->cursor_pos>>3) + 1) << 3; break;
				default :
				{	err->buffer[err->cursor_pos] = (current_color<<8) | *str;
					err->cursor_pos++;
				}
			}
			str++;
			if (err->cursor_pos >= err->num_col*err->num_row)
			{	// Add a comment for the next line, please.
				memcpyw((word *) err->buffer, (word *) err->buffer + (err->num_col<<1), err->num_col * err->num_row);
				err->cursor_pos -= err->num_col;
			}
		}
		err->vid->redraw(err->buffer, err->cursor_pos);
	}

	void Term_Std::puts(char *str)
	{	if (out == 0) return;
		while (*str)
		{	switch (*str)
			{	case '\n' : out->cursor_pos = (out->cursor_pos/out->num_col + 1) * out->num_col; break;
				case '\r' : out->cursor_pos = (out->cursor_pos/out->num_col) * out->num_col; break;
				case '\t' : out->cursor_pos = ((out->cursor_pos>>3) + 1) << 3; break;
				default :
				{	out->buffer[out->cursor_pos] = (current_color<<8) | *str;
					out->cursor_pos++;
				}
			}
			str++;
			if (out->cursor_pos >= out->num_col*out->num_row)
			{	memcpyw((word *) out->buffer, (word *) out->buffer + (out->num_col<<1), out->num_row * out->num_col);
				out->cursor_pos -= out->num_col;
			}
		}
		out->vid->redraw(out->buffer, out->cursor_pos);
	}

	char *Term_Std::gets(char *str)
	{	int ch;
		char *start_str;
		start_str = str;
		while ((ch = in->read()))
		{	if (ch == -1) continue;
			*str = (char) ch;
			switch (*str)
			{	case '\n' :
				{	out->cursor_pos = (out->cursor_pos/out->num_col + 1) * out->num_col;
					*str = '\0';
					return(start_str);
				}
				case '\t' : out->cursor_pos = ((out->cursor_pos>>3) + 1) << 3; break;
				case '\b' :
				{	if (str != start_str) { str--; out->cursor_pos--; }
					out->buffer[out->cursor_pos] = (current_color<<8) | 0x20;
					*str = '\0';
					str--;
				} break;
				default :
				{	out->buffer[out->cursor_pos] = (current_color<<8) | *str;
					out->cursor_pos++;
				}
			}
			out->vid->redraw(out->buffer, out->cursor_pos);
			str++;
			if (out->cursor_pos >= out->num_col*out->num_row)
			{	memcpyw((word *) out->buffer, (word *) out->buffer + (out->num_col<<1), out->num_row * out->num_col);
				out->cursor_pos -= out->num_col;
			}
		}
		return(start_str);
	}

	void Term_Std::set_active()
	{	out->vid->current_buffer = out->buffer;
		out->vid->redraw(out->buffer, out->cursor_pos);
	}

	void Term_Std::set_color(byte color)
	{	current_color = color;
	}
}
