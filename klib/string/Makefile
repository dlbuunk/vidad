COMMON_FLAGS = -Wall -Wextra -O0 -pedantic
CFLAGS = $(COMMON_FLAGS) -nostdlib -nostartfiles -nodefaultlibs
CPPFLAGS = $(CFLAGS) -fno-builtin -fno-exceptions -fno-rtti -fno-stack-protector -I../include
CC = gcc
CPP = g++

all: constfuncs.o constructors.o destructor.o mutablefuncs.o operators.o libstring.a

constfuncs.o : constfuncs.cpp
	$(CPP) $(CPPFLAGS) -c -o constfuncs.o constfuncs.cpp
constructors.o : constructors.cpp
	$(CPP) $(CPPFLAGS) -c -o constructors.o constructors.cpp
destructor.o : destructor.cpp
	$(CPP) $(CPPFLAGS) -c -o destructor.o destructor.cpp
mutablefuncs.o : mutablefuncs.cpp
	$(CPP) $(CPPFLAGS) -c -o mutablefuncs.o mutablefuncs.cpp
operators.o : operators.cpp
	$(CPP) $(CPPFLAGS) -c -o operators.o operators.cpp
libstring.a : constfuncs.o constructors.o destructor.o mutablefuncs.o operators.o constfuncs.cpp constructors.cpp destructor.cpp mutablefuncs.cpp operators.cpp 
	ar rcs libstring.a constfuncs.o constructors.o destructor.o mutablefuncs.o operators.o 

tests.o : tests.cpp
	$(CPP) $(COMMON_FLAGS) -std=c++0x -c -o tests.o tests.cpp
test : tests.o libstring.a tests.cpp constfuncs.cpp constructors.cpp destructor.cpp mutablefuncs.cpp operators.cpp 
	$(CPP) $(COMMON_FLAGS) -o test tests.o -lgtest -L. -lstring -L../lib -lcstring

.PHONY: clean
clean:
	rm -f *.o libstring.a test

