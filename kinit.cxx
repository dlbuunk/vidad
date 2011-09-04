#include "kernel.h"

dword const volatile ktime = 0;
dword boot_time;

extern "C" void kinit(int (*_loader_read)(byte *, int), void (*_loader_puts)(char *), void (*_loader_timer)(dword), void (*_loader_exit)(), MemInfo * meminfo, Info * info, word * gdtp, word * idtp)
{
	// we don't need the bootloader timer & floppy driver, stop them
	(*_loader_exit)();

	// start the kernel logger
	klog_init(0, _loader_puts);

	kprint("Let's test 0x%X.", 0x19AF);

	// unused variables
	(void) _loader_read;
	(void) _loader_timer;
	(void) meminfo;
	(void) info;
	(void) gdtp;
	(void) idtp;
}

