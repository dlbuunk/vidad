CF = -w -nostdlib -nostartfiles -nodefaultlibs
CCF = -fno-builtin -fno-exceptions -fno-rtti -fno-stack-protector $(CF)

all: kbc.o key_translate.o keyboard.o

kbc.o: kbc.cc
	gcc $(CCF) -c -o kbc.o kbc.cc

keyboard.o: keyboard.cc
	gcc $(CCF) -c -o keyboard.o keyboard.cc

key_translate.o: key_translate.cc
	gcc $(CCF) -c -o key_translate.o key_translate.cc


.PHONY: clean
clean:
	rm -f *.o
