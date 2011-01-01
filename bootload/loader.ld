ENTRY(loader_entry)
OUTPUT_FORMAT(binary)
INPUT(loader.o)
OUTPUT(loader.bin)
SECTIONS
{	. = 0x7E00 ;
	.text :
	{	*(.text*)
	}
}
