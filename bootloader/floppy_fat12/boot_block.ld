ENTRY(boot_block_entry)

OUTPUT_FORMAT(binary)

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
