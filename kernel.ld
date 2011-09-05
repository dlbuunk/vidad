ENTRY(kinit)
OUTPUT_FORMAT(binary)
STARTUP(pointer.o)
INPUT
(
	kinit.o
	kprint.o
	malloc.o
)
OUTPUT(kernel.bin)
SECTIONS
{
	. = 0x10000;

	.text :
	{
		*(.text*)
		*(.gnu.linkonce.t*)
	}

	.rodata :
	{
		start_ctors = .;
		*(.ctor*)
		end_ctors = .;

		start_dtors = .;
		*(.dtor*)
		end_dtors = .;

		*(.rodata*)
		*(.gnu.linkonce.r*)
	}

	.data :
	{
		*(.data*)
		*(.gnu.linkonce.d*)
	}

	.bss :
	{
		start_bss = .;
		*(COMMON*)
		*(.bss*)
		*(.gnu.linkonce.b*)
		end_bss = .;
	}
}

