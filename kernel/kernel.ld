ENTRY(kmain)
OUTPUT_FORMAT(binary)
STARTUP(pointer.o)
INPUT
(
	kmain.o
)
OUTPUT(kernel.bin)
SECTIONS
{	. = 0x00010000;
	.text :
	{	*(.text*)
	}
}
