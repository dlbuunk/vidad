LD = /usr/local/cross/bin/i386-elf-ld
AS = /usr/local/cross/bin/i386-elf-as

all: image
	bochs -q

image: fs.s fs.ld
	make -C bootloader
	$(AS) -o fs.o fs.s
	$(LD) -T fs.ld
	chmod -x fs.bin
	cp bootloader/boot_block.bin image
	dd if=fs.bin of=image seek=8
	dd if=bootloader/page_init.bin of=image seek=24
	dd if=/dev/zero of=image seek=29 count=2851

.PHONEY: clean
clean:
	make -C bootloader clean
	rm -rf image *.{o,tmp,bin}
