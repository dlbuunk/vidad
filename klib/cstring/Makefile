include ../../local.mk

all: cstring.o libcstring.a

cstring.o : cstring.cpp
	$(CXX) $(CXXFLAGS) -c -o cstring.o cstring.cpp
libcstring.a : cstring.o cstring.cpp
	ar rcs libcstring.a cstring.o

tests.o : tests.cpp
	$(CXX) $(COMMON_FLAGS) -std=c++0x -c -o tests.o tests.cpp
test : tests.o libcstring.a tests.cpp cstring.cpp
	$(CXX) $(COMMON_FLAGS) -o test tests.o -lgtest -L. -lcstring

installlib : libcstring.a
	cp libcstring.a ../../lib/

.PHONY: clean
clean:
	rm -f *.o libcstring.a test ../lib/libcstring.a

