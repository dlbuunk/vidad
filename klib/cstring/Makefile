COMMON_FLAGS = -Wall -Wextra -O0 -pedantic
CFLAGS = $(COMMON_FLAGS) -nostdlib -nostartfiles -nodefaultlibs -nostdinc
CPPFLAGS = $(CFLAGS) -fno-builtin -fno-exceptions -fno-rtti -fno-stack-protector -std=gnu++0x -I../include
CC = gcc
CPP = g++

all: cstring.o libcstring.a

cstring.o : cstring.cpp
	$(CPP) $(CPPFLAGS) -c -o cstring.o cstring.cpp
libcstring.a : cstring.o cstring.cpp
	ar rcs libcstring.a cstring.o

tests.o : tests.cpp
	$(CPP) $(COMMON_FLAGS) -std=c++0x -c -o tests.o tests.cpp
test : tests.o libcstring.a tests.cpp cstring.cpp
	$(CPP) $(COMMON_FLAGS) -o test tests.o -lgtest -L. -lcstring

installlib : libcstring.a
	cp libcstring.a ../lib/

.PHONY: clean
clean:
	rm -f *.o libcstring.a test ../lib/libcstring.a

