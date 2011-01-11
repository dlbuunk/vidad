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
#ifndef CSTRING_HPP
#define CSTRING_HPP

#include <cstddef>

//! \brief Namespace for the standard library used in vidad.
namespace klib {

//! Returns the number of characters until the first \0 in C string pointed to
//! by cstrPtr.
size_t strlen( char const* cstrPtr ); //++
//! \brief Compares the C strings cstrA and cstrB.
//! 
//! Once a pair is found where the two chars are unequal, a positive number is
//! returned if the greater char was reached from cstrA, and a negative if it
//! was reached from cstrB. If all chars are equal, a zero is returned.
int strcmp( char const* cstrA, char const* cstrB ); //++
//! Copies all the characters up to and including the terminating \\0 from
//! C string src to C string dest.
char* strcpy( char* dest, char const* src ); //++
//! Copies the first num bytes of src to dest.
//!
//! \note It is not advised to use this function if the two areas overlap, and
//!       destination is higher in memory than source.
void* memcpy( void* dest, void const* src, size_t num ); //++
//! Makes a byte-by-byte copy of the first num characters of src, placing them
//! in the same order in dest.
//!
//! It is safe to use this on overlapping area memories, no matter their
//! position relative to each other.
void* memmove( void* dest, void const* src, size_t num ); //++
//! \brief Compares the first num bytes of memA and memB.
//! 
//! Once a pair is found where the two bytes are unequal, a positive number is
//! returned if the greater byte was reached from memA, and a negative if it
//! was reached from memB. If all bytes are equal, a zero is returned.
int memcmp( void const* memA, void const* memB, size_t num ); //++

} // namespace klib

#endif // CSTRING_HPP

