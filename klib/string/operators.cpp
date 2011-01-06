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
	return *this;
}

string& string::operator+=( char const* str ) {
	return *this;
}

string& string::operator+=( const char c ) {
	return *this;
}

string& string::operator+=( const unsigned int num ) {
	return *this;
}

string string::operator+( string const& str ) const {
	return string;
}

string string::operator+( char const* str ) const {
	return string;
}

string string::operator+( const char c ) const {
	return string;
}

string string::operator+( const unsigned int num ) const {
	return string;
}

} // namespace klib
