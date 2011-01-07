DEBUG_FLAGS = -Wall -Wextra -O0 -pedantic
CFLAGS      = $(DEBUG_FLAGS) -nostdlib -nostartfiles -nodefaultlibs -nostdinc
CXXFLAGS    = $(CFLAGS) -fno-builtin -fno-exceptions -fno-rtti -fno-stack-protector -std=gnu++0x -I/root/vidad/include
CC          = gcc
CXX         = g++

all: constfuncs.o constructors.o destructor.o mutablefuncs.o operators.o libstring.a

constfuncs.o : constfuncs.cpp string.h
	$(CXX) $(CXXFLAGS) -c -o constfuncs.o constfuncs.cpp
constructors.o : constructors.cpp string.h
	$(CXX) $(CXXFLAGS) -c -o constructors.o constructors.cpp
destructor.o : destructor.cpp string.h
	$(CXX) $(CXXFLAGS) -c -o destructor.o destructor.cpp
mutablefuncs.o : mutablefuncs.cpp string.h
	$(CXX) $(CXXFLAGS) -c -o mutablefuncs.o mutablefuncs.cpp
operators.o : operators.cpp string.h
	$(CXX) $(CXXFLAGS) -c -o operators.o operators.cpp
libstring.a : constfuncs.o constructors.o destructor.o mutablefuncs.o operators.o constfuncs.cpp constructors.cpp destructor.cpp mutablefuncs.cpp operators.cpp string.h inline.h
	ar rcs libstring.a constfuncs.o constructors.o destructor.o mutablefuncs.o operators.o 

tests.o : tests.cpp string.h inline.h
	$(CXX) $(DEBUG_FLAGS) -std=c++0x -c -o tests.o tests.cpp
test : tests.o libstring.a tests.cpp constfuncs.cpp constructors.cpp destructor.cpp mutablefuncs.cpp operators.cpp string.h inline.h
	$(CXX) $(DEBUG_FLAGS) -o test tests.o -lgtest -L. -lstring -L../lib -lcstring

installlib : libstring.a
	cp libstring.a ../lib/

.PHONY: clean
clean:
	rm -f *.o libstring.a test ../lib/libstring.a

