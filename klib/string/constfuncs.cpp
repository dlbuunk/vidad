//==---==  klib/string/constfuncs.cpp ==---------------==  *- C++ -*  ==---==>
//
// This source file contains the string functions that do not modify the
// string (mostly queries about the size and state).
//
//==-----------------------------------------------------------------------==>
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

bool string::empty() const {
	// If the string isn't faked, check whether it's full of \0s.
	if ( allocSize_ ) { 
		for( size_t a = 0; a < strSize_; ++a ) {
			if( strPtr_[a] )
				return false;
		}
	}
	return true;
}

size_t string::length() const {
	return strSize_ - 1;
}

size_t string::size() const {
	return strSize_;
}

size_t string::capacity() const {
	return allocSize_;
}

} // namespace klib
