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

string& string::insert( char c, size_t pos ) {
	if( pos >= strSize_ )
		// Out of bounds
		return *this;
	if( !allocSize_ ) {
		// Empty string...
		allocSize_ = roundto_;
		strPtr_ = new char[allocSize_];
		strSize_++;
		strPtr_[0] = c;
		strPtr_[1] = '\0';
		return *this;
	}
	if( strSize_++ == allocSize_ ) {
		// If necessary, get us more space.
		allocSize_ += 32;
		char* tmpPtr = new char[allocSize_];
		// And copy stuff over, adding the char.
		memcpy( tmpPtr, strPtr_, pos );
		tmpPtr[pos] = c;
		strcpy( tmpPtr + pos + 1, strPtr_ + pos );
		delete strPtr_;
		strPtr_ = tmpPtr;
	} else {
		// Move everything one up.
		memmove( strPtr_ + pos+1,
		         strPtr_ + pos  ,
		         strSize_ - pos - 1 );
		// Don't forget to write the char, too!
		strPtr_[pos] = c;
	}
	return *this;
}

} // namespace klib
