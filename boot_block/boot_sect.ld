ENTRY(_bootsect_entry)
OUTPUT_FORMAT(binary)
INPUT(boot_sect.o)
OUTPUT(boot_sect.tmp)
SECTIONS
{	. = 0x7C00;
	.text :
	{	*(.text*)
	}
}
