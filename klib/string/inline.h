//==---==  klib/string/inline.h  ==--------------------==  *- C++ -*  ==---==>
//
// This header file contains the inline functions of the string class.
//
//==-----------------------------------------------------------------------==>

inline char string::operator[]( size_t pos ) const {
	return '\0';
}

inline char& string::operator[]( size_t pos ) {
	return nullval_;
}

inline bool string::operator==( string const& str ) const {
	return false;
}

inline bool string::operator==( char const* str ) const {
	return false;
}

inline bool string::operator!=( string const& str ) const {
	return false;
}

inline bool string::operator!=( char const* str ) const {
	return false;
}

