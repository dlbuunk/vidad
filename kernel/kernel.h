/* this is a list of all functions in the global namespace
* first come the C function, then the C++ functions
* there are two global asm functions:
* exec_start
* handle_int
*/

/* typedefs used everywhere in the C/C++ code */
typedef unsigned char byte;
typedef unsigned short int word;
typedef unsigned int dword;
typedef unsigned long int qword;

/* color definitions used everywhere */
#define BLACK 0
#define BLUE 1
#define GREEN 2
#define RED 4
#define BROWN 6
#define W 7
#define GRAY 8
#define YELLOW 14
#define WHITE 15

#define WHITE_BLUE 0x1F
#define WHITE_GREEN 0x2F
#define WHITE_RED 0x4F

#ifdef __cplusplus
extern "C" {
#endif

/* functions to be found in helper.s */

extern void memstob(dword addr, dword count, byte val);
extern void memstow(dword addr, dword count, word val);

extern void memcpyb(dword to, dword count, dword from);
extern void memcpyw(dword to, dword count, dword from);

extern byte inportb(word port);
extern void outportb(word port, byte val);

/* data to be found in kernel_data.c */
extern const byte system_font8[];
extern const byte system_font16[];
extern byte vga_mode_t[];
extern byte vga_mode2[];
extern byte vga_mode3[];
extern byte vga_mode4[];
extern byte vga_mode5[];

/* functions and data to be found in kprint.c */

extern word kprint_pos;

extern void kprint(char ch, byte color);
extern void kprints(char *str, byte color);

/* these three are scheduled for removal */
extern void kprintb(byte val, byte color);
extern void kprintw(word val, byte color);
extern void kprintd(dword val, byte color);

/* functions and data to be found in malloc.c */

extern dword alloc_base;
extern dword alloc_top;
extern void alloc_init(dword heap_base, dword top_of_mem);
extern void alloc_error();

extern void *malloc(dword size);
extern void *calloc(dword n, dword size);
extern void free(void *block);

/* functions and data to be found in inter.c */

#define NUM_ENTRIES 2
extern dword inter_callback[16][NUM_ENTRIES][2];
extern void inter_main(byte int_num);

extern void inter_init();
extern int inter_reg(dword addr, dword obj, dword irq);
extern void inter_dereg(dword irq, int num);

/* kernel header struct definition */
struct kheader
{	char magic[6];
	byte video_mode;
	byte cpu_type;
	word mem_low;
	word mem_mid;
	word mem_high;
	word mem_alt;
	byte version;
	byte subversion;
	word reserved;
	dword size;
	word GDT_start;
	word GDT_lenght;
	word IDT_start;
	word IDT_length;
	dword start_of_image;
	dword start_of_code;
	dword modsum;
	dword checksum;
};

/* function in kmain.c */
extern void kmain(struct kheader *kh, dword stack_base);

/* C++ functions with C linkage style in ccentry.cc */
extern void __cxa_pur_virual();
extern void ccentry(struct kheader *kh, dword dma_buff);
extern void kerror(char *str, byte color);

/* also in ccentry.cc :
* overloads of new, new[], delete, and delete[]
*/

#ifdef __cplusplus
};
#undef NUM_ENTRIES

/* functions in printf.cc */

extern char print_buffer[];
extern void print(char *str, char **form);
extern void sprintf(char *str, char *format, ...);
extern void printf(char *format, ...);

#ifndef IO_H
/* IO namespace class definitions, shortened form */
/* I know that this is a bad way of doing things - David */
namespace IO
{	class Init
	{	public : Init() {};
		public : ~Init() {};
		public : void main();
	};

	class Terminal
	{	public : Terminal() {};
		public : virtual ~Terminal() {};
		public : virtual void puts_err(char *str) = 0;
		public : virtual void puts(char *str) = 0;
		public : virtual char *gets(char *str) = 0;
	};
};

extern IO::Terminal *kterm;

#endif /* IO_H */

#endif /* __cplusplus */
