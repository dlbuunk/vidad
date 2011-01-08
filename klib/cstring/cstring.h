//==---==  klib/cstring/cstring.h ==---------------------==  *- C++ -*  ==---==>
//
// Standard header, contains the functions that are usually in cstring.
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
//==-------------------------------------------------------------------------==>
#ifndef CSTRING_H
#define CSTRING_H

#include <cstddef>
namespace klib {

// All these functions behave as the C standard requires them to.
size_t strlen( char const* cstr ); //++
int strcmp( char const* cstrA, char const* cstrB ); //++
char* strcpy( char* dest, char const* src ); //++
void* memcpy( void* dest, void const* src, size_t num ); //++
void* memmove( void* dest, void const* src, size_t num ); //-

} // namespace klib

#endif // CSTRING_H

