ENTRY(boot_sect_entry)

OUTPUT_FORMAT(binary)

INPUT(boot_sect.o)

OUTPUT(boot_sect.tmp)

SECTIONS
{
	. = 0x00000600;

	.text :
	{
		*(.text)
	}
}
