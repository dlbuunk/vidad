#include "kernel.h"

void kmain(struct kheader *kh, dword stack_base)
{	word memsize;

	memstow(0xB8000, 2000, 0x0720);
	kprint_pos = 0;
	kprints("SYSTEM LOADED\n", 0x01);

	/* initialise interrupt handling code */
	inter_init();

	/* set the memsize in 32 KB units */
	memsize = kh->mem_low >> 5;

	/* we need 32 KB for heap and 32 KB for DMA buffer */
	if (memsize < (stack_base>>15) + 2)
	{	kprints("Error: not enough low memory, ViDad will not run this system.\n", 0x04);
		return;
	}

	/* init the heap, but leave enough space for a 32 KB DMA buffer */
	alloc_init(stack_base, (memsize-1)<<15);

	/* call the c++ entry function */
	ccentry(kh, (memsize-1)<<15);
};
