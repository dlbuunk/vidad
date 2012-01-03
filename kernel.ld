ENTRY(__kernel_entry)

OUTPUT_FORMAT(binary)

STARTUP(kernel_entry.o)

INPUT
(
	int_entry.o
	c_entry.o
	cxx_entry.o
	util/util.o
	system/system.o
	memory/memory.o
	thread/thread.o
	/* stream/stream.o */
	/* event/event.o */
	/* process/process.o */
	/* io/io.o */
)

OUTPUT(kernel.bin)

SECTIONS
{
	. = 0x00100000;

	.text :
	{
		*(.text)
		*(.gnu.linkonce.t*)
	}

	.rodata :
	{
		ctor_start = . ;
		*(.ctor*)
		ctor_end = . ;

		dtor_start = . ;
		*(.dtor*)
		dtor_end = . ;

		*(.rodata*)
		*(.gnu.linkonce.r*)
	}

	.data :
	{
		*(.data)
		*(.gnu.linkonce.d*)
	}

	.bss :
	{
		bss_start = . ;
		*(COMMON)
		*(.bss)
		*(.gnu.linkonce.b*)
		bss_end = . ;
	}

	/DISCARD/ :
	{
		*(.comment)
		*(.eh_frame)
	}
}
