extern "C" {
#include "helper.h"
#include "kprint.h"
#include "malloc.h"
#include "kheader.h"
};

extern "C" int inter_reg(dword func, dword obj, dword irq);
extern "C" void inter_dereg(dword irq, int num);

extern "C" void kerror(char *str, byte color);

extern void sprintf(char *str, char *format, ...);
extern void printf(char *format, ...);

extern "C" const byte system_font8[];
extern "C" const byte system_font16[];
extern "C" byte vga_mode_t[];
extern "C" byte vga_mode2[];
extern "C" byte vga_mode3[];
extern "C" byte vga_mode4[];
extern "C" byte vga_mode5[];
