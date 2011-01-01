all: force_look
	make -C kernel
	cp kernel/kernel.bin .
	make -C bootload
	cp bootload/bootload.bin .
	cp bootload/loader.bin .
	dd if=bootload.bin of=image
	dd if=loader.bin of=image seek=1
	dd if=kernel.bin of=image seek=2

write:
	dd if=image of=/dev/fd0u1440

.PHONY: clean
clean:
	rm -f *.bin image
	make -C bootload clean
	make -C kernel clean

.PHONY: force_look
force_look:
	true
