#include "kernel.h"

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
{	kerror("Fatal error: cannot make pure virtual function call\n", WHITE_GREEN);
};

// kernel entry function
extern "C" void ccentry(struct kheader *kh, dword dma_buff)
{	(void) kh;
	(void) dma_buff;
	kterm = 0; // do this in order to make things like 'if (kterm)' work
	IO::Init *init = new IO::Init;
	init->main();
	delete init;
};

// function to print out fatal error messages
// TODO: make compatible with kernel terminal
extern "C" void kerror(char *str, byte color)
{	memstow(0xB8000, 2000, 0x0020 | (color<<8));
	kprint_pos = 0;
	kprints(str, color);
	kprints("\nSYSTEM HALTED", color);
	asm("cli");
	asm("hlt");
};
