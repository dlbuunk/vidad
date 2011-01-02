ENTRY(bootload_entry)
OUTPUT_FORMAT(binary)
INPUT(bootload.o)
OUTPUT(bootload.tmp)
SECTIONS
{	.text :
	{	*(.text*)
	}
}
