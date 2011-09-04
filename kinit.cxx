#include "kernel.h"

dword const volatile ktime = 0;
dword boot_time;

extern "C" void kinit(int (*_loader_read)(byte *, int), void (*_loader_puts)(char *), void (*_loader_timer)(dword), void (*_loader_exit)(), MemInfo * meminfo, Info * info, word * gdtp, word * idtp)
{
	// clear .bss
	for (dword * i = &start_bss; i < &end_bss; i++)
		*i = 0;

	// start the kernel logger
	klog_init(0, _loader_puts);

	kprint("ctors from 0x%X to 0x%X", &start_ctors, &end_ctors);
	kprint("dtors from 0x%X to 0x%X", &start_dtors, &end_dtors);
	kprint("  bss from 0x%X to 0x%X", &start_bss, &end_bss);

	// unused variables
	(void) _loader_exit;
	(void) _loader_read;
	(void) _loader_timer;
	(void) meminfo;
	(void) info;
	(void) gdtp;
	(void) idtp;
}

