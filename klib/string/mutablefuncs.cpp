//==---==  klib/string/mutablefuncs.cpp ==---------------==  *- C++ -*  ==---==>
//
// This file contains the functions that modify the string somehow.
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
//==-------------------------------------------------------------------------==>
#include "string.h"

namespace klib {

void string::reserve( size_t size ) {
	(void)size;
}

void string::clear() {
}

void string::drop() {
}

void string::validate() {
	// If the string has a \0 somewhere, truncate at it.
	if( !strPtr_ )
		return;
	size_t len = strlen( strPtr_ );
	if( len + 1 != strSize_ )
		truncateAt( len );
}

string& string::truncateAt( size_t pos ) {
	if( pos >= strSize_ || !strPtr_ )
		return *this;
	strPtr_[pos] = '\0';
	strSize_ = pos + 1;
	return *this;
}

void string::insert( char const* cstrPtr, size_t pos ) {
	if( pos >= strSize_ )
		// Out of bounds
		return;
	if( !allocSize_ ) {
		strSize_ += strlen( cstrPtr );
		allocSize_ = ( strSize_ & ~(roundto_ - 1) ) + roundto_;
		strPtr_ = new char[allocSize_];
		strcpy( strPtr_, cstrPtr );
		return;
	}
	size_t len = strlen( cstrPtr );
	if( strSize_ + len > allocSize_ ) {
		strSize_ += len; // We don't need the original size anymore.
		allocSize_ = ( strSize_ & ~(roundto_ - 1) ) + roundto_;
		char* tmpPtr = new char[allocSize_];
		memcpy( tmpPtr, strPtr_, pos );
		strcpy( tmpPtr + pos, cstrPtr );
		strcpy( tmpPtr + pos + len, strPtr_ + pos );
		delete[] strPtr_;
		strPtr_ = tmpPtr;
	} else {
		// Move things up:
		memmove( strPtr_ + pos + len, // By len
		         strPtr_ + pos  , // From the position inserted at.
		         strSize_ - pos ); // This number of chars.
		// Can't use strcpy here, as it would add a \0.
		memcpy( strPtr_ + pos, cstrPtr, len );
		strSize_ += len;
	}
}

void string::insert( char c, size_t pos ) {
	if( pos >= strSize_ )
		// Out of bounds
		return;
	if( !allocSize_ ) {
		// Empty string, allocate some.
		allocSize_ = roundto_;
		strPtr_ = new char[allocSize_];
		strSize_++;
		strPtr_[0] = c;
		strPtr_[1] = '\0';
		return;
	}
	if( strSize_++ == allocSize_ ) {
		// If necessary, get us more space.
		allocSize_ += 32;
		char* tmpPtr = new char[allocSize_];
		// And copy stuff over, adding the char in the middle.
		memcpy( tmpPtr, strPtr_, pos );
		tmpPtr[pos] = c;
		strcpy( tmpPtr + pos + 1, strPtr_ + pos );
		delete[] strPtr_;
		strPtr_ = tmpPtr;
	} else {
		// Move everything one up.
		memmove( strPtr_ + pos + 1,
		         strPtr_ + pos  ,
		         strSize_ - pos - 1 );
		// Don't forget to write the char, too!
		strPtr_[pos] = c;
	}
}

} // namespace klib
