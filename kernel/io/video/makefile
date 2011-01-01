CF = -w -nostdlib -nostartfiles -nodefaultlibs
CCF = -fno-builtin -fno-exceptions -fno-rtti -fno-stack-protector $(CF)

all: vga.o vga_text.o

vga.o: vga.cc
	gcc $(CCF) -c -o vga.o vga.cc

vga_text.o: vga_text.cc
	gcc $(CCF) -c -o vga_text.o vga_text.cc

.PHONY: clean
clean:
	rm -f *.o