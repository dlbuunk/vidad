LD = /usr/local/cross/bin/i386-elf-ld
AS = /usr/local/cross/bin/i386-elf-as

all: fs.s fs.ld
	make -C boot_block
	$(AS) -o fs.o fs.s
	$(LD) -T fs.ld
	chmod -x fs.bin
	cp boot_block/boot_block.bin image
	dd if=fs.bin of=image seek=8
	dd if=/dev/zero of=image seek=24 count=2856
	bochs -q

.PHONEY: clean
clean:
	make -C boot_block clean
	rm -rf image *.{o,tmp,bin}
