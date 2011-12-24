LD = /usr/local/cross/bin/i386-elf-ld
AS = /usr/local/cross/bin/i386-elf-as

boot_block.bin: boot_sect.tmp boot_block.tmp
	cp boot_sect.tmp boot_block.bin
	dd if=boot_block.tmp of=boot_block.bin seek=1

boot_block.tmp: boot_block.s boot_block.ld
	$(AS) -o boot_block.o boot_block.s
	$(LD) -T boot_block.ld
	chmod -x boot_block.tmp

boot_sect.tmp: boot_sect.s boot_sect.ld
	$(AS) -o boot_sect.o boot_sect.s
	$(LD) -T boot_sect.ld
	chmod -x boot_sect.tmp

.PHONEY: clean
clean :
	rm -rf *.{o,tmp,bin}
