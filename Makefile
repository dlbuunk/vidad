include kernel.mk

all: image
	bochs -q

image: fs.s fs.ld kernel.bin
	make -C bootloader
	$(AS) -o fs.o fs.s
	$(LD) -T fs.ld
	chmod -x fs.bin
	cp bootloader/boot_block.bin image
	dd if=fs.bin of=image seek=8
	dd if=bootloader/page_init.bin of=image seek=24
	dd if=kernel.bin of=image seek=32
	dd if=/dev/zero of=image seek=2879 count=1

kernel.bin: kernel_entry.o int_entry.o
	$(LD) -T kernel.ld
	chmod -x kernel.bin

kernel_entry.o: kernel_entry.s
	$(AS) -o kernel_entry.o kernel_entry.s

int_entry.o:
	$(CXX) $(CXXFLAGS) -c -o int_entry.o int_entry.cxx

.PHONEY: clean
clean:
	make -C bootloader clean
	rm -rf image *.{o,tmp,bin}
