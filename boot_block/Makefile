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
	as -o boot_sect.o boot_sect.s
	ld -T boot_sect.ld
	chmod -x boot_sect.tmp

boot_block.tmp: boot_block.s loader.o
	as -o boot_block.o boot_block.s
	ld -T boot_block.ld
	chmod -x boot_block.tmp

loader.o: loader.c
	gcc -Wall -Wextra -Werror -nostdlib -nostartfiles -nodefaultlibs -nostdinc -Os -S -o loader.s loader.c
	as -o loader.o loader.s

