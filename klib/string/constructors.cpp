#include "string.h"

namespace klib {

string::string( size_t res ) {
	(void)res;
}

string::string( string const& str, size_t res ) { 
	(void)str;
	(void)res;
}

string::string( char const* str, size_t num, size_t res ) {
	(void)str;
	(void)num;
	(void)res;
}

} // namespace klib
