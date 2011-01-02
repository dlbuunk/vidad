//==---==  klib/string/operators.h  ==-----------------==  *- C++ -*  ==---==>
//
// This source file contains the operators that for some reason haven't been
// made inline.
//
//==-----------------------------------------------------------------------==>
#include "string.h"

namespace klib {

string& string::operator=( string const& str ) {
	return *this;
}

string& string::operator=( char const* str ) {
	return *this;
}

string& string::operator+=( string const& str ) {
	// Size of one plus size of the other minus one because both are
	// null-terminated.
	if( strSize_ + str.strSize_ - 1 > strAlloc_ ) {
		tmpPtr_ = new char[ strSize_ + str.strSize_ - 1 ];
		strAlloc_ = strSize_ += str.strSize_ - 1;
		strcpy( tmpPtr_, strPtr_ );
		// The following should copy starting from last char of the
		// current string.
		strcpy( tmpPtr_ + strSize_ - 1, str.strPtr_ );
		delete[] strPtr_;
		strPtr_ = tmpPtr_;
		return *this;
	}
	strcpy( tmpPtr_ + strSize_ - 1, str.strPtr_ );
	return *this;
}

string& string::operator+=( char const* str ) {
	int sz = strlen( str );
	if( strSize_ + sz - 1 > strAlloc_ ) {
		tmpPtr_ = new char[ strSize_ + str.strSize_ - 1 ];
		strAlloc_ = strSize_ += str.strSize_ - 1;
		strcpy( tmpPtr_, strPtr_ );
		// The following should copy starting from last char of the
		// current string.
		strcpy( tmpPtr_ + strSize_ - 1, str.strPtr_ );
		delete[] strPtr_;
		strPtr_ = tmpPtr_;
		return *this;
	}
	strcpy( tmpPtr_ + strSize_ - 1, str.strPtr_ );
	return *this;
}

string& string::operator+=( const char c ) {
	// If they're equal, appending in place isn't an option.
	if( strSize_ == strAlloc_ ) {
		// Don't increment strSize_ yet, in case the new fails.
		tmpPtr_ = new char[strSize_+1];
		// Copy the string to the new array.
		strcpy( tmpPtr_, strPtr_ );
		tmpPtr[strSize_++] = c; // Increment is here
		tmpPtr[strSize_] = '\0';
		strAlloc_ = strSize_;
		delete[] strPtr_;
		strPtr_ = tmpPtr_;
		return *this;
	}
	strPtr[strSize_++] = c;
	strPtr[strSize_] = '\0';
	return *this;
}

string& string::operator+=( const unsigned int num ) {
	return *this;
}

string string::operator+( string const& str ) const {
}

string string::operator+( char const* str ) const {
}

string string::operator+( const char c ) const {
}

string string::operator+( const unsigned int num ) const {
}

} // namespace klib
