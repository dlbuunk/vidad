ENTRY(_boot_block_init)
OUTPUT_FORMAT(binary)
INPUT
(
	boot_block.o
	loader.o
)
OUTPUT(boot_block.tmp)
SECTIONS
{
	. = 0x1200;
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
	}
}

