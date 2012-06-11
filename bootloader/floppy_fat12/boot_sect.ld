ENTRY(boot_sect_entry)

OUTPUT_FORMAT(binary)

SECTIONS
{
	. = 0x00000600;

	.text :
	{
		*(.text)
	}
}
