ENTRY(multiboot_entry)

OUTPUT_FORMAT(binary)

SECTIONS
{
	. = 0x00101000;

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
		*(COMMON)
		*(.bss)
		bss_end = . ;
	}
}
