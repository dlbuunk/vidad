//==---==  klib/string/constructors.cpp ==-------------==  *- C++ -*  ==---==>
//
// This source file contains the constructors for the klib::string class.
//
//==-----------------------------------------------------------------------==>
#include "string.h"
#include <cstring>

namespace klib {

string::string( size_t res ) :
    nullval_( '\0' ) {
	if( res ) { // Has reserving memory been requested?
		// User has requested that the string reserve a certain amount
		// of memory, give it to him.
		strPtr_ = new char[res];
		strPtr_[0] = '\0';
	} else {
		// User has requested no allocation, or said nothing, fake it.
		strPtr_ = 0;
	}
	strSize_ = 1; // For the \0
	allocSize_ = res;
}

// This constructor is almost identical to the one from a C string, so see that
// one for the code's comments.
string::string( string const& str, size_t res ) :
    nullval_( '\0' ) { 
	strSize_ = str.strSize_;
	if( res ) { 
		if( res < strSize_ ) 
			res = strSize_;
	} else {
		res = ( strSize_ & ~(roundto_ - 1) ) + roundto_;
	}
	strPtr_ = new char[res];
	allocSize_ = res;
	if( str.strPtr_ )
		memcpy( strPtr_, str.strPtr_, strSize_ );
	else
		strPtr_[0] = '\0';
}

string::string( char const* cstrPtr, size_t res ) :
    nullval_( '\0' ) {
	strSize_ = strlen( cstrPtr ) + 1;
	if( res ) { // If the user requested a certain amount of memory...
		if( res < strSize_ ) // ... but not enough.
			res = strSize_;
	} else {// No request, let's store the amount we need in res anyway.
		// Round up to roundto_. If already a multiple of roundto_, add
		// another roundto_ just to be sure. You could see this code as
		// res = (strSize / roundto) * roundto + roundto;
		// Please note that this is much faster, but that it needs
		// roundto to be a power of 2 to work.
		res = ( strSize_ & ~(roundto_ - 1) ) + roundto_;
	}
	// Okay, done with preparation, create everything.
	// At the moment:
	// - res is the size of the allocated memory
	// - strSize_ is the length of the string
	strPtr_ = new char[res];
	allocSize_ = res;
	strcpy( strPtr_, cstrPtr );
}

} // namespace klib

