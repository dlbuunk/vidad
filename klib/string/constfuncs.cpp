//==---==  klib/string/constfuncs.cpp ==---------------==  *- C++ -*  ==---==>
//
// This source file contains the string functions that do not modify the
// string (mostly queries about the size and state).
//
//==-----------------------------------------------------------------------==>
#include "string.h"

namespace klib {

const char* string::c_str() const {
	if( !strPtr_ ) { // If we don't have a string, let's make one.
		strPtr_ = new char[roundto_]; // Allowed thanks to mutable
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

string string::subStr( size_t pos, size_t len ) const {
	if( pos >= strSize_ ) 
		// User has requested something past the end of the string.
		return string();
	if( len && pos + len >= strSize_ )
		// User has asked for more chars than we can give, so
		// we'll just give him all.
		len = 0;
	string tmp( c_str() + pos ); 
	if( len ) // If the user wants it truncated...
		tmp.truncateAt( len );
	return tmp;
}

} // namespace klib
