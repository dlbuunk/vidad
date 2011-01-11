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
#include "string.h"
#include <cstring>

namespace klib {

string::string( size_t res ) :
    nullval_( '\0' )  {
	if( res ) { // Has reserving memory been requested?
		// User has requested that the string reserve a certain amount
		// of memory, give it to him.
		strPtr_ = new char[res];
	} else {
		// User has requested no allocation, or said nothing, fake it.
		strPtr_ = 0;
	}
	strSize_ = 0;
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
	if( strSize_ )
		memcpy( strPtr_, str.strPtr_, strSize_ );
}

string::string( char const* cstrPtr, size_t res ) :
    nullval_( '\0' )  {
	strSize_ = strlen( cstrPtr );
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
	memcpy( strPtr_, cstrPtr, strSize_ );
}

} // namespace klib

