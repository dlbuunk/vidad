ENTRY(fs_entry)

OUTPUT_FORMAT(binary)

INPUT(fs.o)

OUTPUT(fs.bin)

SECTIONS
{
	.text :
	{
		*(.text)
	}
}
