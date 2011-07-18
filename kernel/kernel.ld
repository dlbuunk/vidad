ENTRY(kmain)
OUTPUT_FORMAT(binary)
STARTUP(pointer.o)
INPUT
(
	helper.o
	malloc.o
	kmain.o
)
OUTPUT(kernel.bin)
SECTIONS
{	. = 0x00010000;
	.text :
	{
		*(.text*)
	}
	.rodata :
	{
		*(.rodata*)
	}
	.data :
	{
		*(.data*)
	}
	.bss :
	{
		*(COMMON)
		*(.bss*)
		end_bss = .;
	}
}
