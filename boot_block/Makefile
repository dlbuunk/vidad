LD = /usr/local/cross/bin/i386-elf-ld
AS = /usr/local/cross/bin/i386-elf-as
CC = /usr/local/cross/bin/i386-elf-gcc
CCFLAGS = -Wall -Wextra -Werror \
	-nostdlib -fno-builtin -nostartfiles -nodefaultlibs -Os -std=gnu99

boot_block.bin: boot_sect.tmp boot_block.tmp
	cp boot_sect.tmp boot_block.bin
	dd if=boot_block.tmp of=boot_block.bin seek=1

boot_block.tmp: boot_block.ld boot_block.o loader_hw.o
	$(LD) -T boot_block.ld
	chmod -x boot_block.tmp

boot_block.o: boot_block.s
	$(AS) -o boot_block.o boot_block.s

loader_hw.o: loader_hw.c
	$(CC) $(CCFLAGS) -c -o loader_hw.o loader_hw.c

boot_sect.tmp: boot_sect.ld boot_sect.s
	$(AS) -o boot_sect.o boot_sect.s
	$(LD) -T boot_sect.ld
	chmod -x boot_sect.tmp

.PHONEY: clean
clean :
	rm -rf *.{o,tmp,bin}
