include ../../local.mk

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

string.h.gen : string.h
	cpp -P -o string.h.gen -I$(ROOT_DIR)/include/ string.h
tests_new.o : tests.cpp string.h string.h.gen inline.h
	$(CXX) $(COMMON_FLAGS) -std=c++0x -c -o tests_new.o tests.cpp -DSKIPSUCCESSFUL
testnew : tests_new.o libstring.a tests.cpp constfuncs.cpp constructors.cpp destructor.cpp mutablefuncs.cpp operators.cpp string.h inline.h
	$(CXX) $(COMMON_FLAGS) -o testnew tests_new.o -lgtest -L. -L$(ROOT_DIR)/lib/ -lcstring -lstring 
tests_all.o : tests.cpp string.h string.h.gen inline.h
	$(CXX) $(COMMON_FLAGS) -std=c++0x -c -o tests_all.o tests.cpp
testall : tests_all.o libstring.a tests.cpp constfuncs.cpp constructors.cpp destructor.cpp mutablefuncs.cpp operators.cpp string.h inline.h
	$(CXX) $(COMMON_FLAGS) -o testall tests_all.o -lgtest -L. -L$(ROOT_DIR)/lib/ -lcstring -lstring 

installlib : libstring.a
	cp libstring.a $(ROOT_DIR)/lib/

.PHONY: clean
clean:
	rm -f *.o string.h.gen libstring.a $(ROOT_DIR)/lib/libstring.a testnew testall

