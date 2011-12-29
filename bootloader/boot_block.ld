ENTRY (boot_block_entry)

OUTPUT_FORMAT(binary)

INPUT(
	boot_block.o
	loader_hw.o
	loader_m.o
)

OUTPUT(boot_block.tmp)

SECTIONS
{
	. = 0x00001200;

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
