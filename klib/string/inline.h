//==---==  klib/string/inline.h  ==--------------------==  *- C++ -*  ==---==>
//
// This header file contains the inline functions of the string class.
//
//==-----------------------------------------------------------------------==>

inline const char& string::operator[]( size_t pos ) const {
	(void)pos;
	return nullval_;
}

inline char& string::operator[]( size_t pos ) {
	(void)pos;
	return nullval_;
}

inline bool string::operator==( string const& str ) const {
	(void)str;
	return false;
}

inline bool string::operator==( char const* str ) const {
	(void)str;
	return false;
}

inline bool string::operator!=( string const& str ) const {
	(void)str;
	return false;
}

inline bool string::operator!=( char const* str ) const {
	(void)str;
	return false;
}

