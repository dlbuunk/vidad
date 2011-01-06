COMMON_FLAGS = -Wall -Wextra -O0 -pedantic
CFLAGS = $(COMMON_FLAGS) -w -nostdlib -nostartfiles -nodefaultlibs
CPPFLAGS = $(COMMON_FLAGS) $(CFLAGS) -fno-builtin -fno-exceptions -fno-rtti -fno-stack-protector
CC = gcc
CPP = g++

all: constfuncs.o constructors.o destructor.o mutablefuncs.o operators.o staticfuncs.o libstring.a

constfuncs.o :
	$(CPP) $(CPPFLAGS) -c -o constfuncs.o constfuncs.cpp
constructors.o :
	$(CPP) $(CPPFLAGS) -c -o constructors.o constructors.cpp
destructor.o :
	$(CPP) $(CPPFLAGS) -c -o destructor.o destructor.cpp
mutablefuncs.o :
	$(CPP) $(CPPFLAGS) -c -o mutablefuncs.o mutablefuncs.cpp
operators.o :
	$(CPP) $(CPPFLAGS) -c -o operators.o operators.cpp
staticfuncs.o :
	$(CPP) $(CPPFLAGS) -c -o staticfuncs.o staticfuncs.cpp
libstring.a : constfuncs.o constructors.o destructor.o mutablefuncs.o operators.o staticfuncs.o
	ar rcs libstring.a constfuncs.o constructors.o destructor.o mutablefuncs.o operators.o staticfuncs.o

tests.o :
	$(CPP) $(COMMON_FLAGS) -std=c++0x -c -o tests.o tests.cpp
test : tests.o
	$(CPP) $(COMMON_FLAGS) -o test tests.o -lgtest -L. -lstring

.PHONY: clean
clean:
	rm -f *.o libstring.a test

