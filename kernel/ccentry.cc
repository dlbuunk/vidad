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

#include <string>
using klib::string;

// kernel terminal
IO::Terminal *kterm;

// overload new and delete
void *operator new(size_t size)
{	return calloc(1, size);
}
void *operator new[](size_t size)
{	return calloc(1, size);
}
void operator delete(void *p)
{	return free(p);
}
void operator delete[](void *p)
{	return free(p);
}

// back up function for pure virtual functions
extern "C" void __cxa_pure_virtual()
{	kerror("Fatal error: cannot make pure virtual function call\n", 0x2F);
}

// kernel entry function
extern "C" void ccentry(struct kheader *kh, dword dma_buff)
{	(void) kh;
	(void) dma_buff;
	kterm = 0; // do this in order to make things like 'if (kterm)' work
	IO::Init *init = new IO::Init;
	char buf[180];
	for (;;) { kterm->puts(kterm->gets(buf)); kterm->puts("\n"); }
//	delete init;
}

// function to print out fatal error messages
extern "C" void kerror(char *str, byte color)
{	IO::Terminal *term;
	term = kterm;
	kterm = 0; // so that the next call to kerror (error in error handling) uses the basic method.
	if (term)
	{	term->set_color(color);
		term->puts_err(str);
		term->puts_err("\nSYSTEM HALTED");
	}
	else
	{	memstow((word *) 0xB8000, 0x0020 | (color<<8), 2000);
		// An explanantion of these numbers would be very nice.
		kprint_pos = 0;
		kprints(str, color);
		kprints("\nSYSTEM HALTED", color);
	}
	asm("cli");
	asm("hlt");
}

void print(string const& str)
{	if (kterm) kterm->puts((char *) str.c_str());
	else kprints(str.c_str(), 0x07);
}

void print(char const *cstr)
{	print(string(cstr));
}
