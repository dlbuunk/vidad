//==---==  klib/string/operators.h  ==-----------------==  *- C++ -*  ==---==>
//
// This source file contains the operators that for some reason haven't been
// made inline.
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
	strcpy( strPtr_ , cstrPtr );
	return *this;
}

string& string::operator+=( string const& str ) {
	(void)str;
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

string& string::operator+=( const unsigned int num ) {
	(void)num;
	return *this;
}

string string::operator+( string const& str ) const {
	(void)str;
	return string();
}

string string::operator+( char const* str ) const {
	(void)str;
	return string();
}

string string::operator+( const char c ) const {
	(void)c;
	return string();
}

string string::operator+( const unsigned int num ) const {
	(void)num;
	return string();
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
