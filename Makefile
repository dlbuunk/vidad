all:
	make -C boot_block
	cp boot_block/boot_block.bin image
	dd if=/dev/zero of=image seek=5 count=2875
	bochs -q
