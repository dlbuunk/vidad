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

string::string( string const& str, size_t res ) { 
	allocSize_ = 0;
	strPtr_ = 0;
	strSize_ = 9001;
	(void)str;
	(void)res;
}

string::string( char const* cstrPtr, size_t num, size_t res ) :
    nullval_( '\0' ) {
	if( num ) {
		// The user wants a specific number of characters copied.
		strSize_ = num; // This'll be the size of the final string.

		// If res is set, we should allocate as little space as we can.
		// Used to simplify the if structure.
		bool minsize = res;

		// Is the last of those characters a \0?
		bool nullterm = !cstrPtr[num-1];

		// res is just used to store the amount of memory necessary.
		if( nullterm ) {
			// The string to be copied is null-terminated, no need
			// to append anything, so we can just copy num chars.
			if( res < num )
				// Make sure he didn't ask for too little
				// memory...
				res = num;
		} else {
			// The string isn't null-terminated, we need one extra
			// char.
			if( res <= num )
				res = num + 1;
			++strSize_; // Need to add one character for \0
		}
		// Okay, we now know that res is large enough. Should we add
		// some for later?
		if( !minsize ) {
			// Round up to roundto_. If already a multiple of
			// roundto_, add another roundto_ just to be sure.
			// You can see this code as:
			// res = (res / roundto) * roundto + roundto;
			// Please note that this is much faster, but that it
			// needs roundto to be a power of 2 to work.
			res = ( res & ~(roundto_ - 1) ) + roundto_;
		}
		// Okay, done with preparation, create everything.
		// At the moment:
		// - res is the size of the allocated memory
		// - num is the length of the string
		// Suggestions on how to use strSize_ instead of num are
		// welcome, seeing as that's what's used if num is not
		// specified (it avoids one copy).
		strPtr_ = new char[res];
		allocSize_ = res;
		memcpy( strPtr_, cstrPtr, num );
		if( !nullterm )
			// The string is not null-terminated, let's fix that.
			strPtr_[num] = '\0';
		return; // No point in hanging around.	
	}
	// The user wants us to copy until a \0 is hit.
	strSize_ = strlen( cstrPtr ) + 1;
	if( res ) { // If the user requested a certain amount of memory...
		if( res < strSize_ ) // If the user requested too little memory
			res = strSize_;
	} else { // No request, let's store the amount we need in res anyway.
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
	memcpy( strPtr_, cstrPtr, strSize_ );
}

} // namespace klib

