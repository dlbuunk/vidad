/* This file is part of Vidad.
*
*  Vidad is free software: you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  Vidad is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with Vidad.  If not, see <http://www.gnu.org/licenses/>.
*/

/* this is a list of all functions in the global namespace
* first come the C function, then the C++ functions
* there are two global asm functions:
* exec_start
* handle_int
*/

#include <cstddef>

#ifdef __cplusplus
extern "C" {
#endif

/* functions to be found in helper.s */

extern void memstob(byte *addr, byte val, size_t count);
extern void memstow(word *addr, word val, size_t count);

extern void memcpyb(byte *to, byte *from, size_t count);
extern void memcpyw(word *to, word *from, size_t count);

extern byte inportb(word port);
extern void outportb(word port, byte val);

/* data to be found in kernel_data.c */
extern byte keyboard_mode1[];
extern byte keyboard_mode2[];
extern word keyboard_mode3[];
extern byte keyboard_shift[];

extern const byte system_font8[];
extern const byte system_font16[];
extern byte vga_mode_t[];
extern byte vga_mode2[];
extern byte vga_mode3[];
extern byte vga_mode4[];
extern byte vga_mode5[];
extern byte vga_mode6[];
extern byte vga_mode7[];
extern byte vga_mode8[];
extern byte vga_mode9[];

/* functions and data to be found in kprint.c */

extern word kprint_pos;

extern void kprint(char ch, byte color);
extern void kprints(char *str, byte color);

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
}
#undef NUM_ENTRIES

/* functions in printf.cc */

extern char print_buffer[];
extern void print(char *str, char **form);
extern void sprintf(char *str, char *format, ...);
extern void printf(char *format, ...);

#endif /* __cplusplus */
