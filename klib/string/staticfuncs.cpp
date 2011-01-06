#include "string.h"

namespace klib {

static size_t strlen( char const* str ) {
	(void)str;
	return 0;
}

static char* strcpy( char* dest, char const* src ) {
	(void)src;
	return dest;
}

static char* strncpy( char* dest, char const* src, size_t num ) {
	(void)src;
	(void)num;
	return dest;
}

static char* strcmp( char* dest, char const* src ) {
	(void)src;
	return dest;
}

} // namespace klib

