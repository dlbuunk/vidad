LD = /usr/local/cross/bin/i386-elf-ld
AS = /usr/local/cross/bin/i386-elf-as

boot_sect.bin : boot_sect.s boot_sect.ld
	$(AS) -o boot_sect.o boot_sect.s
	$(LD) -T boot_sect.ld
	chmod -x boot_sect.bin

.PHONEY: clean
clean :
	rm -rf boot_sect.{o,bin}
