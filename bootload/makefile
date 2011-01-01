all: bootload.bin loader.bin

bootload.bin: bootload.ld bootload.s
	as -o bootload.o bootload.s
	ld -T bootload.ld
	chmod -x bootload.bin

loader.bin: loader.ld loader.s
	as -o loader.o loader.s
	ld -T loader.ld
	chmod -x loader.bin

.PHONY: clean
clean:
	rm -f *.o *.bin
