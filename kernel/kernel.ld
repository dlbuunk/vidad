ENTRY(exec_start)
OUTPUT_FORMAT(binary)
INPUT(
	startup.o
	helper.o
	kernel_data.o
	kernel.o

	../lib/libstring.a
	../lib/libcstring.a
)
OUTPUT(kernel.tmp)
SECTIONS
{	. = 0x8000 ;
	.text ALIGN(512) :
	{	*(.text*)
		*(.gnu.linkonce.t*)
	}
	.rodata ALIGN(512) :
	{	*(.rodata*)
		*(.gnu.linkonce.r*)
	}
	.data ALIGN(512) :
	{	start_ctors = . ;
		*(.ctor*)
		end_ctors = . ;
		start_dtors = . ;
		*(.dtor*)
		end_dtors = . ;
		*(.data*)
		*(.gnu.linkonce.d*)
	}
	.bss ALIGN(512) :
	{	*(.COMMON*)
		*(.bss*)
		*(.gnu.linkonce.b*)
	}
}
