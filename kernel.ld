ENTRY(__kernel_entry)

OUTPUT_FORMAT(binary)

INPUT(kernel_entry.o)

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
