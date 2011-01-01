CF = -w -nostdlib -nostartfiles -nodefaultlibs
CCF = -fno-builtin -fno-exceptions -fno-rtti -fno-stack-protector $(CF)

kernel.bin: addsum kernel.ld startup.o helper.o vga_data.o kprint.o malloc.o inter.o kmain.o ccentry.o force_look
	make -C io
	ld -T kernel.ld
	chmod -x kernel.tmp
	./addsum kernel.tmp kernel.bin

addsum: addsum.c
	gcc -o addsum addsum.c

startup.o: startup.S header.inc gdt.inc idt.inc handle_int.inc
	cpp -o startup.s startup.S
	as -o startup.o startup.s

helper.o: helper.s
	as -o helper.o helper.s

vga_data.o: vga_data.c
	gcc -S -o vga_data.s vga_data.c
	as -o vga_data.o vga_data.s

kprint.o: kprint.c
	gcc $(CF) -S -o kprint.s kprint.c
	as -o kprint.o kprint.s

malloc.o: malloc.c
	gcc $(CF) -S -o malloc.s malloc.c
	as -o malloc.o malloc.s

inter.o: inter.c
	gcc $(CF) -S -o inter.s inter.c
	as -o inter.o inter.s

kmain.o: kmain.c
	gcc $(CF) -S -o kmain.s kmain.c
	as -o kmain.o kmain.s

ccentry.o: ccentry.cc
	gcc $(CCF) -S -o ccentry.s ccentry.cc
	as -o ccentry.o ccentry.s

.PHONY : clean
clean:
	rm -f addsum kernel.{tmp,bin} helper.o {startup,vga_data,malloc,kprint,inter,kmain,ccentry}.s *.o
	make -C io clean

.PHONY: force_look
force_look:
	true
