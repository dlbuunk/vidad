CF = -w -nostdlib -nostartfiles -nodefaultlibs
CCF = -fno-builtin -fno-exceptions -fno-rtti -fno-stack-protector $(CF)

all: kbc.o

kbc.o: kbc.cc
	gcc $(CCF) -c -o kbc.o kbc.cc

.PHONY: clean
clean:
	rm -f *.o
