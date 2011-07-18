CFLAGS = -O3 -march=i386 -Wall -Wextra -Wno-parentheses -Werror -nostdlib -nostartfiles -nodefaultlibs -nostdinc

all: kernel.bin

write: kernel.bin
	dd if=kernel.bin of=/dev/fd0u1440 seek=8

.PHONEY: clean
clean:
	rm -f *.{bin,o}

kernel.bin: kernel.ld pointer.o helper.o malloc.o kmain.o
	ld -T kernel.ld
	chmod -x kernel.bin

pointer.o: pointer.s
	as -o pointer.o pointer.s

kmain.o: kmain.c
	gcc $(CFLAGS) -c -o kmain.o kmain.c

malloc.o: malloc.c
	gcc $(CFLAGS) -c -o malloc.o malloc.c

helper.o: helper.c
	gcc $(CFLAGS) -c -o helper.o helper.c
