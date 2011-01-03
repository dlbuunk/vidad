ENTRY(exec_start)
OUTPUT_FORMAT(binary)
INPUT(
	startup.o
	helper.o
	kernel_data.o
	kprint.o
	malloc.o
	inter.o
	kmain.o
	ccentry.o

	io/init.o
	io/pic.o
	io/timer.o
	io/video/vga.o
	io/video/vga_text.o
	io/terminal/term_buf.o
	io/terminal/term_std.o
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
