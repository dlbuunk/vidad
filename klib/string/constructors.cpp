#include "string.h"

namespace klib {

string::string() :
    strPtr_( 0 ), strSize_( 0 ), strAlloc_( 0 ), nullval_( 0 )  {
}

string::string( string const& str ) : 
    strSize_( str.strSize_ ), strAlloc_( str.strSize_ ), nullval_( 0 ) {
	strPtr_ = new char[strSize_];
	strcpy( strPtr_, str.strPtr_ );
}

string::string( char const* str ) :
    nullval_( 0 ) {
	strAlloc_ = strSize_ = strlen( str );
	strPtr_ = new char[strSize_];
	strcpy( strPtr_, str );
}

} // namespace klib
