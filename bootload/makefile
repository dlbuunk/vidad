bootload.bin: bootload.ld bootload.s
	as -o bootload.o bootload.s
	ld -T bootload.ld
	chmod -x bootload.tmp
	dd if=bootload.tmp of=bootload.bin skip=48 seek=0 count=1
	dd if=bootload.tmp of=bootload.bin skip=63 seek=1 count=1

.PHONY: clean
clean:
	rm -f *.o *.tmp *.bin
