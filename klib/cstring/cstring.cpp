//==---==  klib/cstring/cstring.cpp ==-------------------==  *- C++ -*  ==---==>
//
//! \file klib/cstring/cstring.cpp
//! \brief This source file defines the functions generally present in cstring.
//!
// Copyright:
//   This file is part of vidad::klib.
//
//   vidad::klib is free software: you can redistribute it and/or modify
//   it under the terms of the GNU Lesser General Public License as
//   published by the Free Software Foundation, either version 3 of the
//   License, or (at your option) any later version.
//
//   vidad::klib is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU Lesser General Public License for more details.
//
//   You should have received a copy of the GNU Lesser General Public License
//   along with vidad::klib.  If not, see <http://www.gnu.org/licenses/>.
//
//==-------------------------------------------------------------------------==>
#include <cstddef>
#include "cstring.hpp"

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

char* strcpy( char* dest, char const* src ) {
	char* orig = dest;
	while(( *dest++ = *src++ )); 
	return orig;
}

void* memcpy( void* dest, void const* src, size_t num ) {
	byte* destp = reinterpret_cast< byte* > ( dest );
	byte const* srcp = reinterpret_cast< byte const* > ( src );
	while( num-- )
		*(destp++) = *(srcp++); 
	return dest;
}

void* memmove( void* dest, void const* src, size_t num ) {
	if( dest == src )
		return dest;
	byte* destp = reinterpret_cast< byte* > ( dest );
	byte const* srcp = reinterpret_cast< byte const* > ( src );
	if( dest < src ) {
		// If destination is below source, we can just copy.
		while( num-- )
			*(destp++) = *(srcp++); 
	} else {
		// If it is above the source, however, we need to reverse-copy
		// Set the pointers to the top of where they should copy.
		destp += num;
		srcp += num;
		// Have them move downwards. Decrementing destp in the
		// while statement avoids having to decrement after
		// adding num.
		while( --destp >= dest )
			*destp = *(--srcp);
	}
	return dest;
}

int memcmp( void const* memA, void const* memB, size_t num ) {
	byte const* memAp = reinterpret_cast< const byte* > ( memA );
	byte const* memBp = reinterpret_cast< byte const* > ( memB );
	while( num-- ) {
		if( *memAp != *memBp )
			return *memAp - *memBp;
		memAp++;
		memBp++;
	}
	return 0;
}

} // namespace klib

