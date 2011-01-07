all: force_look
	make -C klib
	make -C kernel
	cp kernel/kernel.bin .
	make -C bootload
	cp bootload/bootload.bin .
	dd if=bootload.bin of=image
	dd if=kernel.bin of=image seek=2

write:
	dd if=image of=/dev/fd0u1440

.PHONY: clean
clean:
	rm -f *.bin image
	make -C bootload clean
	make -C kernel clean
	make -C klib clean

.PHONY: force_look
force_look:
	true
