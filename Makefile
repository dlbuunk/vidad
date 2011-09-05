LD = /usr/local/cross/bin/i386-elf-ld
AS = /usr/local/cross/bin/i386-elf-as
CXX = /usr/local/cross/bin/i386-elf-g++ -Wall -Wextra -Werror -nostdlib -fno-builtin -nostartfiles -nodefaultlibs -fno-exceptions -fno-rtti -fno-stack-protector -fno-strict-aliasing -O3 -c

all: vidad.img

vidad.img: kernel.bin
	make -C boot_block boot_block.bin
	cp boot_block/boot_block.bin vidad.img
	dd if=kernel.bin of=vidad.img seek=8

.PHONEY: write
write: vidad.img
	dd if=vidad.img of=/dev/fd0u1440

.PHONEY: clean
clean:
	make -C boot_block clean
	rm -f *.o *.bin *.img

kernel.bin: pointer.o kinit.o kprint.o malloc.o
	$(LD) -T kernel.ld
	chmod -x kernel.bin

pointer.o: pointer.s
	$(AS) -o pointer.o pointer.s

kinit.o: kinit.cxx kernel.h
	$(CXX) -o kinit.o kinit.cxx

kprint.o: kprint.cxx kernel.h
	$(CXX) -o kprint.o kprint.cxx

malloc.o: malloc.cxx kernel.h
	$(CXX) -o malloc.o malloc.cxx

