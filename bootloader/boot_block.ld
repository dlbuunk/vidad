ENTRY(boot_block_entry)

OUTPUT_FORMAT(binary)

STARTUP(boot_block.o)

INPUT(
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
}
