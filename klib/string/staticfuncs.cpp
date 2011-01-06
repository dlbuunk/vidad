//==---==  klib/string/constfuncs.cpp ==---------------==  *- C++ -*  ==---==>
//
// This source file contains the string functions that do not modify the
// string (mostly queries about the size and state).
//
//==-----------------------------------------------------------------------==>
#include "string.h"

namespace klib {

size_t string::strlen( char const* cstr ) {
	size_t a = 0;
	while( cstr[a] )
		a++;
	return a;
}

char* string::strcpy( char* dest, char const* src ) {
	(void)src;
	return dest;
}

char* string::strncpy( char* dest, char const* src, size_t num ) {
	(void)src;
	(void)num;
	return dest;
}

int string::strcmp( char const* cstrA, char const* cstrB ) {
	while( *cstrA && *cstrB ) {
		if( *cstrA != *cstrB )
			return *cstrA - *cstrB;
		cstrA++;
		cstrB++;
	}
	if( *cstrA != *cstrB )
		return *cstrA - *cstrB;
	return 0;
}

} // namespace klib

