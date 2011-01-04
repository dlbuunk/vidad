//==---==  klib/string/inline.h  ==--------------------==  *- C++ -*  ==---==>
//
// This header file contains the inline functions of the string class.
//
//==-----------------------------------------------------------------------==>

inline const char& string::operator[]( size_t pos ) const {
	if( pos < size )
		return strPtr_[pos];
	nullval_ = '\0';
	return nullval_;
}

inline char& string::operator[]( size_t pos ) {
	if( pos < size )
		return strPtr_[pos];
	return nullval_;
}

inline bool string::operator==( string const& str ) const {
	if( strSize_ != str.strSize_ ) // If they're different size, they're obviously different.
		return false;
	if( !strcmp( strPtr_, str.strPtr_ ) )
		return true;
	return false;
}

inline bool string::operator==( char const* str ) const {
	if( !strcmp( strPtr_, str.strPtr_ ) )
		return true;
	return false;
}

inline bool string::operator!=( string const& str ) const {
	if( strSize_ != str.strSize_ ) // If they're different size, they're obviously different.
		return true;
	if( strcmp( strPtr_, str.strPtr_ ) )
		return true;
	return false;
}

inline bool string::operator!=( char const* str ) const {
	if( !strcmp( strPtr_, str.strPtr_ ) )
		return true;
	return false;
}

