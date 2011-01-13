//==---==  klib/string/mutablefuncs.cpp ==---------------==  *- C++ -*  ==---==>
//
// This file contains the functions that modify the string.
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
#include "private_inline.hpp"
#include <cstring>

namespace klib {

void string::reserve( size_t size ) {
	if( size < strSize_ )
		size = strSize_; // Avoid requests that ask for too little.
	if( size == allocSize_ )
		return; // Avoid doing anything if nothing needs to be done.
	allocSize_ = size; // We don't care how much was allocated before.
	if( !strPtr_ ) // In case we didn't have a string.
		strPtr_ = new char[size];
	else
		changeAlloc( size );
}

string& string::drop() {
	delete[] strPtr_;
	strPtr_ = 0;
	allocSize_ = strSize_ = 0;
	return *this;
}

// deprecated
string& string::validate() {
	// If the string has a \0 somewhere, truncate at it.
//	if( !strPtr_ )
//		return *this;
//	size_t len = strlen( strPtr_ );
//	if( len + 1 != strSize_ )
//		truncateAt( len );
	return *this;
}

string& string::truncateAt( size_t pos ) {
	if( pos >= strSize_ )
		return *this;
	strSize_ = pos;
	return *this;
}

string& string::appendDecimal( int val, size_t digits ) {
	if( digits > 10 )
		digits = 10;
	bool neg;
	if( val < 0 ) { // Remember the sign.
		val = -val;
		neg = true;
	} else {
		neg = false;
	}
	char arr[12]; // We'll make a C string of the number.
	arr[11] = '\0';
	arr[0] = '-';
	char* dptr = arr + 10; // Pointer to digit we're working with.
	while( dptr != arr ) {
		// Translate the number to a C string
		// Need to decrement later, to allow space for the '-'
		*(dptr--) = (val % 10) + '0'; // Oh, and convert to digits
		val /= 10;
	}
	// Okay, now let's cut away unnecessary digits.
	// Figuring out where to put the \0:
	char* firstdigit = dptr = arr + 11 - digits;
	while( --dptr != arr ) {
		if( *dptr != '0' ) // Non-zero digit, set it to be the first.
			firstdigit = dptr;
	}
	if( neg ) {
		// Prepend a '-'
		--firstdigit;
		firstdigit[0] = '-';
	}
	*this += firstdigit;
	return *this;
}

string& string::appendHex( unsigned int val, size_t digits ) {
	if( digits > 8 )
		digits = 8;
	char arr[9]; // We'll make a C string of the number.
	arr[8] = '\0';
	size_t a = 8;
	while( a-- ) {
		// Translate the number to a C string
		arr[a] = val & 0xF;
		val >>= 4;
	}
	// Converting to symbols...
	for( a = 0; a < 8; ++a ) {
		if( arr[a] < 9 )
			arr[a] += '0'; // 0 becomes 48, i.e. '0', etc.
		else
			arr[a] += 55; // 10 becomes 65, i.e. 'A', etc.
	}
	// Okay, now let's cut away unnecessary digits.
	// Figuring out where to put the \0:
	char* firstdigit = arr + 8 - digits;
	a = 8 - digits;
	while( a-- ) {
		if( arr[a] != '0' ) // Non-zero digit, set it to be the first.
			firstdigit = arr+a;
	}
	*this += firstdigit;
	return *this;
}

string& string::appendOctal( unsigned int val, size_t digits ) {
	if( digits > 11 )
		digits = 11;
	char arr[12]; // We'll make a C string of the number.
	arr[11] = '\0';
	size_t a = 11;
	while( a-- ) {
		// Translate the number to a C string
		arr[a] = (val & 0x7) + '0'; // And convert right away
		val >>= 3;
	}
	// Okay, now let's cut away unnecessary digits.
	// Figuring out where to put the \0:
	char* firstdigit = arr + 11 - digits;
	a = 11 - digits;
	while( a-- ) {
		if( arr[a] != '0' ) // Non-zero digit, set it to be the first.
			firstdigit = arr+a;
	}
	*this += firstdigit;
	return *this;
}

string& string::appendBinary( unsigned int val, size_t digits ) {
	if( digits > 32 )
		digits = 32;
	char arr[33]; // We'll make a C string of the number.
	arr[32] = '\0';
	size_t a = 32;
	while( a-- ) {
		// Translate the number to a C string
		arr[a] = (val & 0x1) + '0'; // And convert right away
		val >>= 1;
	}
	// Okay, now let's cut away unnecessary digits.
	// Figuring out where to put the \0:
	char* firstdigit = arr + 32 - digits;
	a = 32 - digits;
	while( a-- ) {
		if( arr[a] != '0' ) // Non-zero digit, set it to be the first.
			firstdigit = arr+a;
	}
	*this += firstdigit;
	return *this;
}

// The insert functions use several memcpys to speed things up, and thus it is
// not possible to use the changeAlloc function.
void string::insert( string const& str, size_t pos ) {
	if( pos == strSize_ ) {
		// This can be done as an append.
		*this += str;
		return;
	}
	if( pos > strSize_ )
		// Out of bounds
		return;
	if( strSize_ + str.strSize_ > allocSize_ ) {
		allocSize_ = calcAllocSize( strSize_ + str.strSize_);
		char* tmpPtr = new char[allocSize_];
		// Copy the first part of this string.
		memcpy( tmpPtr, strPtr_, pos );
		// Copy the other string.
		memcpy( tmpPtr + pos, str.strPtr_, str.strSize_ );
		// Copy the second part of this string.
		memcpy( tmpPtr + pos + str.strSize_,
		        strPtr_ + pos,
			strSize_ - pos );
		delete[] strPtr_;
		strPtr_ = tmpPtr;
		strSize_ += str.strSize_;
	} else {
		// Move things up:
		// By the length of the other string.
		memmove( strPtr_ + pos + str.strSize_, 
		         strPtr_ + pos  , // From the position inserted at.
		         strSize_ - pos ); // This number of chars.
		// Can't use strcpy here, as it would add a \0.
		memcpy( strPtr_ + pos, str.strPtr_, str.strSize_ );
		strSize_ += str.strSize_;
	}
}

void string::insert( char const* cstrPtr, size_t pos ) {
	// See insert string for more comments, the code is very similar.
	if( pos == strSize_ ) {
		*this += cstrPtr;
		return;
	}
	if( pos > strSize_ )
		return;
	size_t len = strlen( cstrPtr );
	if( strSize_ + len > allocSize_ ) {
		allocSize_ = calcAllocSize(strSize_ + len);
		char* tmpPtr = new char[allocSize_];
		memcpy( tmpPtr, strPtr_, pos );
		memcpy( tmpPtr + pos, cstrPtr, len );
		memcpy( tmpPtr + pos + len, strPtr_ + pos, strSize_ - pos );
		delete[] strPtr_;
		strPtr_ = tmpPtr;
		strSize_ += len;
	} else {
		// Move things up:
		memmove( strPtr_ + pos + len, // By len
		         strPtr_ + pos  , // From the position inserted at.
		         strSize_ - pos ); // Number of chars after pos.
		// Can't use strcpy here, as it would add a \0.
		memcpy( strPtr_ + pos, cstrPtr, len );
		strSize_ += len;
	}
}

void string::insert( char c, size_t pos ) {
	if( pos == strSize_ ) {
		*this += c;
		return;
	}
	if( pos > strSize_ )
		// Out of bounds
		return;
	strSize_++; // We'll be adding a char.
	// No need to worry about an empty string: handled by above code.
	if( strSize_ > allocSize_ ) {
		// If necessary, get us more space.
		allocSize_ += roundto_;
		char* tmpPtr = new char[allocSize_];
		// And copy stuff over, adding the char in the middle.
		memcpy( tmpPtr, strPtr_, pos );
		tmpPtr[pos] = c;
		memcpy( tmpPtr + pos + 1,
		        strPtr_ + pos,
			strSize_ - 1 );
		delete[] strPtr_;
		strPtr_ = tmpPtr;
	} else {
		// Move everything one up.
		memmove( strPtr_ + pos + 1,
		         strPtr_ + pos,
		         strSize_ - pos - 1 ); 
		// strSize is greater than it was.
		// Don't forget to write the char, too!
		strPtr_[pos] = c;
	}
}

void string::removeSubStr( size_t pos, size_t len ) {
	if( pos >= strSize_ || !len ) // All of these means we shouldn't care.
		return;
	if( pos + len >= strSize_ ) { // User asked us to delete too much.
		truncateAt( pos );
		return;
	}
	memmove( strPtr_ + pos,
	         strPtr_ + pos + len,
	         strSize_ - pos - len );
	strSize_ -= len;
	return;
}

} // namespace klib
