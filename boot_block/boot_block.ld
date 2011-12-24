ENTRY (boot_block_entry)

OUTPUT_FORMAT(binary)

INPUT(boot_block.o)

OUTPUT(boot_block.tmp)

SECTIONS
{
	. = 0x00001000;

	.text :
	{
		*(.text)
	}
}
