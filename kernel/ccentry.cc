#include "io/io.h"
#include <string>
using namespace klib;

// kernel terminal
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
{	kerror("Fatal error: cannot make pure virtual function call\n", 0x2F);
};

// kernel entry function
extern "C" void ccentry(struct kheader *kh, dword dma_buff)
{	(void) kh;
	(void) dma_buff;
	kterm = 0; // do this in order to make things like 'if (kterm)' work
	IO::Init *init = new IO::Init;
	string *s = new string("Hello, world!");
	for (int i=0; i<50; i++) printf("%c", (*s)[i]);
	delete s;
	printf("\n");
	char str[160];
	for (;;) { kterm->puts(kterm->gets(str)); kterm->puts("\n"); }
	for (;;) asm("hlt"); // cool down cpu
	delete init;
};

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
	{	memstow(0xB8000, 2000, 0x0020 | (color<<8));
		kprint_pos = 0;
		kprints(str, color);
		kprints("\nSYSTEM HALTED", color);
	}
	asm("cli");
	asm("hlt");
};
