//==---==  klib/string/constructors.cpp ==-------------==  *- C++ -*  ==---==>
//
// This source file contains the constructors for the klib::string class.
//
//==-----------------------------------------------------------------------==>
#include "string.h"

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

string::string( char const* str, size_t num, size_t res ) {
	allocSize_ = 0;
	strPtr_ = 0;
	strSize_ = 9001;
	(void)str;
	(void)num;
	(void)res;
}

} // namespace klib

