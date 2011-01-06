//==---==  klib/cstring/cstring.cpp ==-------------------==  *- C++ -*  ==---==>
//
// This source file contains the string functions that do not modify the
// string (mostly queries about the size and state).
//
//==-------------------------------------------------------------------------==>
#include <cstddef>
#include "cstring.h"

namespace klib {

size_t strlen( char const* cstr ) {
	size_t a = 0;
	while( cstr[a] )
		a++;
	return a;
}

int strcmp( char const* cstrA, char const* cstrB ) {
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

void* memcpy( void* dest, void const* src, size_t num ) {
	char* destp = reinterpret_cast< char* > ( dest );
	char const* srcp = reinterpret_cast< char const* > ( src );
	while( num-- )
		*(destp++) = *(srcp++); 
	return dest;
}

} // namespace klib

