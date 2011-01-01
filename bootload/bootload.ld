ENTRY(bootload_entry)
OUTPUT_FORMAT(binary)
INPUT(bootload.o)
OUTPUT(bootload.bin)
SECTIONS
{	. = 0x6000 ;
	.text :
	{	*(.text*)
	}
}
