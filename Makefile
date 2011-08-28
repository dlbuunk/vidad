LD = /usr/local/cross/bin/i386-elf-ld
AS = /usr/local/cross/bin/i386-elf-as
CXX = /usr/local/cross/bin/i386-elf-g++ -Wall -Wextra -Werror -nostdlib -fno-builtin -nostartfiles -nodefaultlibs -fno-exceptions -fno-rtti -fno-stack-protector -O3 -c

all: kernel.bin

.PHONEY: write
write: kernel.bin
	dd if=kernel.bin of=/dev/fd0u1440 seek=8

.PHONEY: clean
clean:
	rm -f *.o *.bin

kernel.bin: pointer.o kinit.o kprint.o
	$(LD) -T kernel.ld
	chmod -x kernel.bin

pointer.o: pointer.s
	$(AS) -o pointer.o pointer.s

kinit.o: kinit.cxx kernel.h
	$(CXX) -o kinit.o kinit.cxx

kprint.o: kprint.cxx kernel.h
	$(CXX) -o kprint.o kprint.cxx

