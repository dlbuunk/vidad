//==---==  klib/string/mutablefuncs.cpp ==---------------==  *- C++ -*  ==---==>
//
// This file contains the functions that modify the string somehow.
//
//==-------------------------------------------------------------------------==>
#include "string.h"

namespace klib {

void string::reserve( size_t size ) {
	(void)size;
}

void string::clear() {
}

void string::drop() {
}

void string::validate() {
	// If the string has a \0 somewhere, truncate at it.
	if( !strPtr_ )
		return;
	size_t len = strlen( strPtr_ );
	if( len + 1 != strSize_ )
		truncateAt( len );
}

string& string::truncateAt( size_t pos ) {
	if( pos >= strSize_ || !strPtr_ )
		return *this;
	strPtr_[pos] = '\0';
	strSize_ = pos + 1;
	return *this;
}

} // namespace klib
