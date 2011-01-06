#ifndef CSTRING_H
#define CSTRING_H

#include <cstddef>

namespace klib {

// All these functions behave as the C standard requires them to.
size_t strlen( char const* cstr );
int strcmp( char const* cstrA, char const* cstrB );
void* memcpy( void* dest, void const* src, size_t num );

}

#endif // CSTRING_H

