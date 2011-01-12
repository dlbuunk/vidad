//==---==  klib/string/operators.cpp  ==-----------------==  *- C++ -*  ==---==>
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
//==-------------------------------------------------------------------------==>
#include "string.hpp"
#include <cstring>

namespace klib {
// void casts used to shut the bloody thing up.

string& string::operator=( string const& str ) {
	strSize_ = str.strSize_;
	if( strSize_ > allocSize_ ) {
		// If we don't have enough memory, let's make some.
		allocSize_ = calcAllocSize( strSize_ );
		delete[] strPtr_;
		strPtr_ = new char[allocSize_];
	}
	if( strSize_ )
		memcpy( strPtr_, str.strPtr_, strSize_ );
	return *this;
}

string& string::operator=( char const* cstrPtr ) {
	strSize_ = strlen( cstrPtr );
	if( !strSize_ )
		return *this;
	if( strSize_ > allocSize_ ) {
		// If we don't have enough memory, let's make some.
		allocSize_ = calcAllocSize( strSize_ );
		delete[] strPtr_;
		strPtr_ = new char[allocSize_];
	}
	memcpy( strPtr_, cstrPtr, strSize_ );
	return *this;
}

string& string::operator+=( string const& str ) {
	// Note the position of the last character of this string.
	if( strSize_ + str.strSize_ > allocSize_ ) {
		// If we don't have enough memory, let's make some.
		allocSize_ = calcAllocSize( strSize_ + str.strSize_ );
		changeAlloc( allocSize_ );
	}
	memcpy( strPtr_ + strSize_, str.strPtr_, str.strSize_ );
	strSize_ += str.strSize_;
	return *this;
}

string& string::operator+=( char const* cstrPtr ) {
	size_t len = strlen( cstrPtr );
	if( strSize_ + len > allocSize_ ) {
		// If we don't have enough memory, let's make some.
		allocSize_ = calcAllocSize( strSize_ + len );
		changeAlloc( allocSize_ );
	}
	// Can use strcpy because we know the input is a C string.
	strcpy( strPtr_ + strSize_, cstrPtr );
	strSize_ += len;
	return *this;
}

string& string::operator+=( const char c ) {
	if( strSize_ >= allocSize_ ) {
		// If our string is too small, make it bigger.
		allocSize_ += roundto_;
		changeAlloc( allocSize_ );
	}
	strPtr_[strSize_++] = c; 
	return *this;
}

} // namespace klib

