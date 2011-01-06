//==---==  klib/string/mutablefuncs.cpp ==---------------==  *- C++ -*  ==---==>
//
// This file contains the functions that modify the string somehow.
//
//==-------------------------------------------------------------------------==>
#include "string.h"

namespace klib {

const char* string::c_str() {
	if( !strPtr_ ) { // If we don't have a string, let's make one.
		strPtr_ = new char[roundto_];
		strPtr_[0] = '\0';
		allocSize_ = roundto_;
		// strSize_ should already by 1.
	}
	return strPtr_;
}

void string::reserve( size_t size ) {
	(void)size;
}

void string::clear() {
}

void string::drop() {
}

} // namespace klib
