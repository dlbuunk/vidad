include kernel.mk

all: image
	bochs -q

image: bootloader/boot_block.bin bootloader/page_init.bin \
		fs.s fs.ld kernel.bin
	make -C bootloader
	$(AS) -o fs.o fs.s
	$(LD) -T fs.ld
	chmod -x fs.bin
	cp bootloader/boot_block.bin image
	dd if=fs.bin of=image seek=8
	dd if=bootloader/page_init.bin of=image seek=24
	dd if=kernel.bin of=image seek=32
	dd if=/dev/zero of=image seek=2879 count=1

bootloader/boot_block.bin: force_look
	make -C bootloader boot_block.bin

bootloader/page_init.bin: force_look
	make -C bootloader page_init.bin

kernel.bin: kernel_entry.o int_entry.o c_entry.o cxx_entry.o \
		util/util.o memory/memory.o
	$(LD) -T kernel.ld
	chmod -x kernel.bin

kernel_entry.o: kernel_entry.s
	$(AS) -o kernel_entry.o kernel_entry.s

int_entry.o: int_entry.cxx kernel.hxx util/util.hxx
	$(CXX) $(CXXFLAGS) -c -o int_entry.o int_entry.cxx

c_entry.o: c_entry.c
	$(CC) $(CCFLAGS) -c -o c_entry.o c_entry.c

cxx_entry.o: cxx_entry.cxx kernel.hxx util/util.hxx
	$(CXX) $(CXXFLAGS) -c -o cxx_entry.o cxx_entry.cxx

util/util.o: force_look
	make -C util

memory/memory.o: force_look
	make -C memory

.PHONEY: force_look
force_look:
	true

.PHONEY: clean
clean:
	make -C bootloader clean
	make -C util clean
	make -C memory clean
	rm -rf image *.{o,bin}
