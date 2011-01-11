//==---==  klib/string/constfuncs.cpp ==---------------==  *- C++ -*  ==---==>
//
// This source file contains the string functions that do not modify the
// string (mostly queries about the size and state).
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
#include <cstring>

namespace klib {

const char* string::c_str() const {
	if( strSize_ == allocSize_ ) {
		allocSize_ += roundto_;
		char* tmpPtr = new char[allocSize_];
		memcpy( tmpPtr, strPtr_, strSize_ );
		delete[] strPtr_;
		strPtr_ = tmpPtr;
	}
	strPtr_[strSize_] = '\0';
	return strPtr_;
}

bool string::empty() const {
	// Check whether the string is full of \0s.
	for( size_t a = 0; a < strSize_; ++a ) {
		if( strPtr_[a] )
			return false;
	}
	return true;
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
