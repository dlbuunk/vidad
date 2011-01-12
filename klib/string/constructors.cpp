//==---==  klib/string/constructors.cpp ==-------------==  *- C++ -*  ==---==>
//
// This source file contains the constructors for the klib::string class.
//
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
//==-----------------------------------------------------------------------==>
#include "string.hpp"
#include "private_inline.hpp"
#include <cstring>

namespace klib {

inline size_t string::ctorDecideAllocSize( size_t res ) const {
	if( res ) { // If the user requested a certain amount of memory...
		if( strSize_ > res ) // ... but not enough.
			return strSize_;
		else
			return res;
	} else { // No request, calculate it ourselves.
		return calcAllocSize( strSize_ );
	}
}

string::string( size_t res ) :
    nullval_( '\0' )  {
	strSize_ = 0;
	if(( allocSize_ = res )) { // Has reserving memory been requested?
		// User has requested that the string reserve a certain amount
		// of memory, give it to him.
		strPtr_ = new char[allocSize_];
	} else {
		// User has requested no allocation, or said nothing, fake it.
		strPtr_ = 0;
	}
}

string::string( string const& str, size_t res ) :
    nullval_( '\0' ) {
	strSize_ = str.strSize_;
	allocSize_ = ctorDecideAllocSize( res );
	if( allocSize_ ) {
		strPtr_ = new char[allocSize_];
		memcpy( strPtr_, str.strPtr_, strSize_ );
	} else { // Empty string, don't bother allocating.
		strPtr_ = 0;
	}
}

// See the string constructor for comments
string::string( char const* cstrPtr, size_t res ) :
    nullval_( '\0' )  {
	strSize_ = strlen( cstrPtr );
	allocSize_ = ctorDecideAllocSize( res );
	if( allocSize_ ) {
		strPtr_ = new char[allocSize_];
		memcpy( strPtr_, cstrPtr, strSize_ );
	} else {
		strPtr_ = 0;
	}
}

} // namespace klib

