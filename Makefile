all: floppy_fat12

floppy_fat12: kernel.bin bootloader/floppy_fat12/boot_block.bin
	dd if=/dev/zero of=image count=2880
	mkdosfs -f 1 -F 12 -r 16 -R 8 -v image
	dd if=boot_block.bin of=image count=8 conv=notrunc,sync
	sudo mount -o loop -vt msdos image mnt
	sudo cp kernel.bin mnt
	sudo umount mnt

bootloader/floppy_fat12/boot_block.bin: force_look
	scons -C bootloader/floppy_fat12
	cp bootloader/floppy_fat12/boot_block.bin .

kernel.bin: force_look
	scons kernel.bin

.PHONY: force_look
force_look:
	true

.PHONY: clean
clean:
	scons -c
	scons -c -C bootloader/floppy_fat12
	rm -f *.tmp *.bin image
