LD = /usr/local/cross/bin/i386-elf-ld
AS = /usr/local/cross/bin/i386-elf-as
CC = /usr/local/cross/bin/i386-elf-gcc -Wall -Wextra -Werror -nostdlib -nodefaultlibs -nostartfiles -nostdinc -Os -c

all: boot_sect.tmp boot_block.tmp
	cp boot_sect.tmp boot_block.bin
	dd if=boot_block.tmp of=boot_block.bin seek=1

.PHONEY: write
write: all
	dd if=boot_block.bin of=/dev/fd0u1440

.PHONEY: clean
clean:
	rm -f *.{o,tmp,bin} loader.s

boot_sect.tmp: boot_sect.s
	$(AS) -o boot_sect.o boot_sect.s
	$(LD) -T boot_sect.ld
	chmod -x boot_sect.tmp

boot_block.tmp: boot_block.s loader.o
	$(AS) -o boot_block.o boot_block.s
	$(LD) -T boot_block.ld
	chmod -x boot_block.tmp

loader.o: loader.c
	$(CC) -o loader.o loader.c

