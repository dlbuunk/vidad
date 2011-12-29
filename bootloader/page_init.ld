ENTRY(page_init)

OUTPUT_FORMAT(binary)

STARTUP(kheader.o)

INPUT(page_init.o)

OUTPUT(page_init.bin)

SECTIONS
{
	. = 0x0000E000;

	.text :
	{
		*(.text)
	}

	.rodata :
	{
		*(.rodata*)
	}

	.data :
	{
		*(.data)
	}

	.bss :
	{
		bss_start = . ;
		*(COMMON)
		*(.bss)
		bss_end = . ;
	}
}
