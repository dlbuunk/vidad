#include "kernel.h"

dword const volatile ktime = 0;
dword boot_time;

extern "C" void kinit(
	int (*_loader_read)(byte *, int),
	void (*_loader_puts)(char *),
	void (*_loader_timer)(dword),
	void (*_loader_exit)(),
	MemInfo * meminfo,
	Info * info,
	word * gdtp,
	word * idtp)
{
	// clear .bss (MUST be done before ANYTHING else)
	for (dword * i = &start_bss; i < &end_bss; i++)
		*i = 0;

	// start the kernel logger
	klog_init(_loader_puts);

	kprint("Kernel running and .bss cleared.");
	kprint("This is Vidad version 0.01.");

	// start memory manager
	alloc_init((void *)&end_bss, (void *)((dword)info->mem_low << 10));

	// test memory manager
	void *foo, *bar, *baz;
	kprint("malloc test: allocating 6 bytes, addr = 0x%X", foo = malloc(6));
	kprint("malloc test: allocating 9 bytes, addr = 0x%X", malloc(9));
	kprint("malloc test: freeing 6 bytes"); free(foo);
	kprint("malloc test: allocating 12 bytes, addr = 0x%X", malloc(12));
	kprint("malloc test: allocating 31 bytes, addr = 0x%X", malloc(31));
	kprint("malloc test: allocating 95 bytes, addr = 0x%X", foo = malloc(95));
	kprint("malloc_test: allocating 40 bytes, addr = 0x%X", bar = malloc(40));
	kprint("malloc_test: freeing 95 bytes"); free(foo);
	kprint("malloc_test: allocating 96 bytes, addr = 0x%X", foo = malloc(96));
	kprint("malloc_test: freeing 96 bytes"); free(foo);
	kprint("malloc_test: allocating 97 bytes, addr = 0x%X", malloc(97));
	kprint("malloc_test: allocating 32 bytes, addr = 0x%X", foo = malloc(32));
	kprint("malloc_test: allocating 32 bytes, addr = 0x%X", baz = malloc(32));
	kprint("malloc_test: freeing 32 bytes"); free(foo);
	kprint("malloc_test: freeing 40 bytes"); free(bar);
	kprint("malloc_test: freeing 32 bytes"); free(baz);
	kprint("malloc_test: allocating 148 bytes, addr = 0x%X", malloc(148));	

	// unused variables
	(void) _loader_exit;
	(void) _loader_read;
	(void) _loader_timer;
	(void) meminfo;
	(void) gdtp;
	(void) idtp;
}

