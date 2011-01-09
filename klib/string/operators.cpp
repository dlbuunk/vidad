//==---==  klib/string/operators.h  ==-----------------==  *- C++ -*  ==---==>
//
// This source file contains the operators that for some reason haven't been
// made inline.
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

namespace klib {
// void casts used to shut the bloody thing up.

string& string::operator=( string const& str ) {
	strSize_ = str.strSize_;
	if( strSize_ > allocSize_ ) {
		// If we don't have enough memory, let's make some.
		allocSize_ = ( strSize_ & ~(roundto_ - 1) ) + roundto_;
		delete[] strPtr_;
		strPtr_ = new char[allocSize_];
	}
	if( str.strPtr_ )
		memcpy( strPtr_, str.strPtr_, strSize_ );
	else
		strPtr_[0] = '\0';
	return *this;
}

string& string::operator=( char const* cstrPtr ) {
	strSize_ = strlen( cstrPtr ) + 1;
	if( strSize_ > allocSize_ ) {
		// If we don't have enough memory, let's make some.
		delete[] strPtr_;
		allocSize_ = ( strSize_ & ~(roundto_ - 1) ) + roundto_;
		strPtr_ = new char[allocSize_];
	}
	strcpy( strPtr_, cstrPtr );
	return *this;
}

string& string::operator+=( string const& str ) {
	// Note the position of the last character of this string.
	size_t pos = --strSize_;
	strSize_ += str.strSize_;
	if( strSize_ > allocSize_ ) {
		// If we don't have enough memory, let's make some.
		allocSize_ = ( strSize_ & ~(roundto_ - 1) ) + roundto_;
		if( strPtr_ ) {
			// If we have anything, let's copy it over.
			char* tmpPtr = new char[allocSize_];
			strcpy( tmpPtr, strPtr_ );
			delete[] strPtr_;
			strPtr_ = tmpPtr;
		} else {
			// If we don't, just make some memory.
			strPtr_ = new char[allocSize_];
		}
	}
	strcpy( strPtr_ + pos, str.strPtr_ );
	return *this;
}

string& string::operator+=( char const* cstrPtr ) {
	size_t pos = strSize_ - 1;
	strSize_ += strlen( cstrPtr );
	if( strSize_ > allocSize_ ) {
		// If we don't have enough memory, let's make some.
		allocSize_ = ( strSize_ & ~(roundto_ - 1) ) + roundto_;
		if( strPtr_ ) {
			// If we have anything, let's copy it over.
			char* tmpPtr = new char[allocSize_];
			strcpy( tmpPtr, strPtr_ );
			delete[] strPtr_;
			strPtr_ = tmpPtr;
		} else {
			// If we don't, just make some memory.
			strPtr_ = new char[allocSize_];
		}
	}
	strcpy( strPtr_ + pos, cstrPtr );
	return *this;
}

string& string::operator+=( const char c ) {
	if( !allocSize_ ) {
		// If we don't have a string yet, make one.
		allocSize_ = roundto_;
		strPtr_ = new char[allocSize_];
	}
	if( strSize_ >= allocSize_ ) {
		allocSize_ += 32;
		char* tmpPtr = new char[allocSize_];
		strcpy( tmpPtr, strPtr_ );
		delete[] strPtr_;
		strPtr_ = tmpPtr;
	}
	strPtr_[strSize_ - 1 ] = c; 
	strPtr_[strSize_++] = '\0'; // Only increment the size here
	return *this;
}

char& string::operator[]( size_t pos ) {
	if( pos >= strSize_ )
		return nullval_ = 0;
	if( !allocSize_ ) {
		strPtr_ = new char[roundto_];
		allocSize_ = roundto_;
		strPtr_[0] = '\0';
	}
	return strPtr_[pos];
}

} // namespace klib
