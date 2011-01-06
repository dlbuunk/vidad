#include "string.h"

namespace klib {

const char* string::c_str() const {
	// Placeholder
	char *tmpPtr = new char[64];
	tmpPtr[0] = '\0';
	return tmpPtr;
}

bool string::empty() const {
	return false;
}

size_t string::length() const {
	return 0;
}

size_t string::size() const {
	return 0;
}

size_t string::capacity() const {
	return 0;
}

} // namespace klib
