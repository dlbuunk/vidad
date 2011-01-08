//==---==  klib/string/inline.h  ==--------------------==  *- C++ -*  ==---==>
//
// This header file contains the inline functions of the string class.
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

#include <cstring>

namespace klib {

inline const char& string::operator[]( size_t pos ) const {
	if( !allocSize_ )
		return nullval_ = '\0';
	if( pos >= strSize_ )
		return nullval_ = '\0';
	return strPtr_[pos];
}


inline bool string::operator==( string const& str ) const {
	if( strSize_ != str.strSize_ )
		return false;
	if( !allocSize_ )
		return true;
	return !( strcmp( strPtr_, str.strPtr_ ) );
}

inline bool string::operator==( char const* cstr ) const {
	if( strlen( cstr ) != (strSize_ - 1) )
		return false;
	// | If nothing is allocated, and the lengths are equal, both must be
	// v empty.
	if( !allocSize_ ) 
		return true;
	return !( strcmp( strPtr_, cstr ));
}

inline bool string::operator!=( string const& str ) const {
	return !(*this == str);
}

inline bool string::operator!=( char const* cstrPtr ) const {
	return !(*this == cstrPtr);
}

inline string string::operator+( string const& str ) const {
	string tmp( *this );
	tmp += str;
	return tmp;
}

inline string string::operator+( char const* cstrPtr ) const {
	string tmp( *this );
	tmp += cstrPtr;
	return tmp;
}

inline string string::operator+( const char c ) const {
	string tmp( *this );
	tmp += c;
	return tmp;
}

inline string& string::append( string const& str ) {
	return (*this += str );
}

inline string& string::append( char const* cstrPtr ) {
	return (*this += cstrPtr );
}

inline string& string::append( char c ) {
	return (*this += c );
}

} // namespace klib

