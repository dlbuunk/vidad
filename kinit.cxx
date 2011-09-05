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

	// make the kernel logger use new/delete
	klog_init();

	// test memory manager
	alloc_status();

	// unused variables
	(void) _loader_exit;
	(void) _loader_read;
	(void) _loader_timer;
	(void) meminfo;
	(void) gdtp;
	(void) idtp;
}

