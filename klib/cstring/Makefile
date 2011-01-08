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

all: cstring.o libcstring.a

cstring.o : cstring.cpp cstring.h
	$(CXX) $(CXXFLAGS) -c -o cstring.o cstring.cpp
libcstring.a : cstring.o cstring.cpp 
	ar rcs libcstring.a cstring.o

cstring.h.gen : cstring.h
	cpp -P -o cstring.h.gen -I$(ROOT_DIR)/include/ cstring.h
tests.o : tests.cpp cstring.h cstring.h.gen
	$(CXX) $(COMMON_FLAGS) -std=c++0x -c -o tests.o tests.cpp
test : tests.o libcstring.a tests.cpp cstring.cpp cstring.h cstring.h.gen
	$(CXX) $(COMMON_FLAGS) -o test tests.o -lgtest -L. -L$(ROOT_DIR)/lib/ -lcstring

installlib : libcstring.a
	cp libcstring.a $(ROOT_DIR)/lib/

.PHONY: clean
clean:
	rm -f *.o cstring.h.gen libcstring.a $(ROOT_DIR)/lib/libcstring.a test 

