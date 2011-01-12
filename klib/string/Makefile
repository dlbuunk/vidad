# Copyright:
#   This file is part of vidad::klib.
#
#   vidad::klib is free software: you can redistribute it and/or modify
#   it under the terms of the GNU Lesser General Public License as
#   published by the Free Software Foundation, either version 3 of the
#   License, or (at your option) any later version.
#
#   vidad::klib is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU Lesser General Public License for more details.
#
#   You should have received a copy of the GNU Lesser General Public License
#   along with vidad::klib.  If not, see <http://www.gnu.org/licenses/>.
#
include ../../local.mk

all: constfuncs.o constructors.o destructor.o mutablefuncs.o operators.o \
     libstring.a

constfuncs.o : constfuncs.cpp string.hpp
	$(CXX) $(CXXFLAGS) -c -o constfuncs.o constfuncs.cpp
constructors.o : constructors.cpp string.hpp
	$(CXX) $(CXXFLAGS) -c -o constructors.o constructors.cpp
destructor.o : destructor.cpp string.hpp
	$(CXX) $(CXXFLAGS) -c -o destructor.o destructor.cpp
mutablefuncs.o : mutablefuncs.cpp string.hpp
	$(CXX) $(CXXFLAGS) -c -o mutablefuncs.o mutablefuncs.cpp
operators.o : operators.cpp string.hpp
	$(CXX) $(CXXFLAGS) -c -o operators.o operators.cpp
libstring.a : constfuncs.o constructors.o destructor.o mutablefuncs.o \
              operators.o constfuncs.cpp constructors.cpp destructor.cpp \
              mutablefuncs.cpp operators.cpp string.hpp inline.hpp
	ar rcs libstring.a constfuncs.o constructors.o destructor.o mutablefuncs.o operators.o 
	ranlib libstring.a

string.hpp.gen : string.hpp
	cpp -P -o string.hpp.gen -I$(ROOT_DIR)/include/ string.hpp
tests_new.o : tests.cpp string.hpp string.hpp.gen inline.hpp
	$(CXX) $(COMMON_FLAGS) -std=c++0x -c -o tests_new.o tests.cpp -DSKIPSUCCESSFUL
testnew : tests_new.o libstring.a tests.cpp constfuncs.cpp constructors.cpp \
          destructor.cpp mutablefuncs.cpp operators.cpp string.hpp inline.hpp
	$(CXX) $(COMMON_FLAGS) -o testnew tests_new.o -lgtest -L. -L$(ROOT_DIR)/lib/ -lcstring -lstring 
tests_all.o : tests.cpp string.hpp string.hpp.gen inline.hpp
	$(CXX) $(COMMON_FLAGS) -std=c++0x -c -o tests_all.o tests.cpp
testall : tests_all.o libstring.a tests.cpp constfuncs.cpp constructors.cpp \
          destructor.cpp mutablefuncs.cpp operators.cpp string.hpp inline.hpp
	$(CXX) $(COMMON_FLAGS) -o testall tests_all.o -lgtest -L. -L$(ROOT_DIR)/lib/ -lcstring -lstring 

installlib : libstring.a
	cp libstring.a $(ROOT_DIR)/lib/

.PHONY: clean
clean:
	rm -f *.o string.hpp.gen libstring.a $(ROOT_DIR)/lib/libstring.a testnew testall

