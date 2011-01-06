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
	(void)str;
	return *this;
}

string& string::operator=( char const* str ) {
	(void)str;
	return *this;
}

string& string::operator+=( string const& str ) {
	(void)str;
	return *this;
}

string& string::operator+=( char const* str ) {
	(void)str;
	return *this;
}

string& string::operator+=( const char c ) {
	(void)c;
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
		return nullval_;
	if( !allocSize_ ) {
		strPtr_ = new char[roundto_];
		allocSize_ = roundto_;
	}
	return strPtr_[pos];
}

} // namespace klib
