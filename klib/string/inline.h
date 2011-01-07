//==---==  klib/string/inline.h  ==--------------------==  *- C++ -*  ==---==>
//
// This header file contains the inline functions of the string class.
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
	(void)str;
	return false;
}

inline bool string::operator!=( char const* str ) const {
	(void)str;
	return false;
}

} // namespace klib

