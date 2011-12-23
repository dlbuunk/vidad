ENTRY (boot_sect_entry)

OUTPUT_FORMAT(binary)

INPUT(boot_sect.o)

OUTPUT(boot_sect.bin)

SECTIONS
{
	. = 0x00007C00;

	.text :
	{
		*(.text)
	}
}
