//==---==  klib/cstring/cstring.cpp ==-------------------==  *- C++ -*  ==---==>
//
// This source file contains the functions for the C string-related functions.
// All should behave exactly as the standard requires them to.
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
	byte* destp = reinterpret_cast< byte* > ( dest );
	byte const* srcp = reinterpret_cast< byte const* > ( src );
	while( num-- )
		// No need to make another copy of src, so we just ask the
		// compiler to treat it as a char*. Other than that, this
		// next line copies the 8-bit value
		*(destp++) = *(srcp++); 
	return dest;
}

char* strcpy( char* dest, char const* src ) {
	char* orig = dest;
	while(( *dest++ = *src++ )); 
	return orig;
}

}

