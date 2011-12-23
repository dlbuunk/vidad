all:
	make -C boot_block
	cp boot_block/boot_sect.bin image
	dd if=/dev/zero of=image seek=1 count=2879
	bochs -q
