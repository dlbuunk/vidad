CF = -w -nostdlib -nostartfiles -nodefaultlibs
CCF = -fno-builtin -fno-exceptions -fno-rtti -fno-stack-protector $(CF)

all: term_buf.o term_std.o

term_buf.o: term_buf.cc
	gcc $(CCF) -c -o term_buf.o term_buf.cc

term_std.o: term_std.cc
	gcc $(CCF) -c -o term_std.o term_std.cc

.PHONY: clean
clean:
	rm -f *.o
