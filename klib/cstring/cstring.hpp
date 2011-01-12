//==---==  klib/cstring/cstring.hpp ==-------------------==  *- C++ -*  ==---==>
//
//! \file klib/cstring/cstring.hpp
//! \brief Partially standard-compliant cstring header.
//!
//! Contains the functions that are currently implemented. They should all be
//! standard-compliant individually.
//!
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

//! \brief Returns the number of characters until the first \0 in C string pointed to
//!        by cstrPtr.
//!
//! \param cstrPtr is a pointer to a C string.
size_t strlen( char const* cstrPtr ); //++
//! \brief Compares the C strings cstrA and cstrB.
//! 
//! Once a pair is found where the two chars are unequal, a positive number is
//! returned if the greater char was reached from cstrA, and a negative if it
//! was reached from cstrB. If, at some point, both chars are \\0, a zero is
//! returned.
//!
//! \param cstrA is the a pointer to the first C string to be compared.
//! \param cstrB is the a pointer to the second C string to be compared.
int strcmp( char const* cstrA, char const* cstrB ); //++
//! \brief Copies all the characters up to and including the terminating \\0
//!        from C string src to C string dest.
//!
//! \param dest is a pointer to the destination.
//! \param src is a constant pointer to the source.
char* strcpy( char* dest, char const* src ); //++
//! Copies the first num bytes of src to dest.
//!
//! \note It is not advised to use this function if the two areas overlap, and
//!       destination is higher in memory than source.
//! 
//! \param dest is a pointer to the destination.
//! \param src is a constant pointer to the source.
//! \param num is the number of bytes to be copied.
void* memcpy( void* dest, void const* src, size_t num ); //++
//! \brief Makes a byte-by-byte copy of the first num characters of src, placing
//!        them in the same order in dest.
//!
//! It is safe to use this on overlapping area memories, no matter their
//! position relative to each other. 
//!
//! \param dest is a pointer to the destination.
//! \param src is a constant pointer to the source.
//! \param num is the number of bytes to be copied.
void* memmove( void* dest, void const* src, size_t num ); //++
//! \brief Compares the first num bytes of memA and memB.
//! 
//! Once a pair is found where the two bytes are unequal, a positive number is
//! returned if the greater byte was reached from memA, and a negative if it
//! was reached from memB. If all bytes are equal, a zero is returned.
//!
//! \param memA is the a pointer to the first area of memory to be compared.
//! \param memB is the a pointer to the second area of memory  to be compared.
//! \param num is the number of bytes to compare before returning 0.
int memcmp( void const* memA, void const* memB, size_t num ); //++

} // namespace klib

#endif // CSTRING_HPP

