include ../local.mk

kernel.bin: addsum kernel.ld startup.o helper.o kernel_data.o kernel.o
	ld -T kernel.ld
	chmod -x kernel.tmp
	./addsum kernel.tmp kernel.bin

addsum: addsum.c
	$(CC) -o addsum addsum.c

startup.o: startup.S header.inc gdt.inc idt.inc handle_int.inc
	cpp -o startup.s startup.S
	as -o startup.o startup.s

helper.o: helper.s
	as -o helper.o helper.s

kernel_data.o: kernel_data.c
	$(CC) -c -o kernel_data.o kernel_data.c

kernel.o: kprint.o malloc.o inter.o kmain.o ccentry.o
	make -C io
	ld -S -r -o kernel.o kprint.o malloc.o inter.o kmain.o ccentry.o io.o

kprint.o: kprint.c
	$(CC) $(CFLAGS) -c -o kprint.o kprint.c

malloc.o: malloc.c
	$(CC) $(CFLAGS) -c -o malloc.o malloc.c

inter.o: inter.c
	$(CC) $(CFLAGS) -c -o inter.o inter.c

kmain.o: kmain.c
	$(CC) $(CFLAGS) -c -o kmain.o kmain.c

ccentry.o: ccentry.cc
	$(CXX) $(CXXFLAGS) -c -o ccentry.o ccentry.cc

.PHONY : clean
clean:
	rm -f addsum kernel.{tmp,bin} startup.s *.o
	make -C io clean

.PHONY: force_look
force_look:
	true
