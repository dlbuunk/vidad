CF = -w -nostdlib -nostartfiles -nodefaultlibs
CCF = -fno-builtin -fno-exceptions -fno-rtti -fno-stack-protector $(CF)

all: init.o pic.o timer.o force_look
	make -C video
	make -C keyboard
	make -C terminal

init.o: init.cc
	gcc $(CCF) -c -o init.o init.cc

pic.o: pic.cc
	gcc $(CCF) -c -o pic.o pic.cc

timer.o: timer.cc
	gcc $(CCF) -c -o timer.o timer.cc

.PHONY: clean
clean:
	rm -f *.o
	make -C video clean
	make -C keyboard clean
	make -C terminal clean

.PHONY: force_look
force_look:
	true
