//==---==  klib/string/private_inline.hpp  ==------------==  *- C++ -*  ==---==>
//
// This file contains the private inline functions of the string class.
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
#ifndef PRIVATE_INLINE_HPP
#define PRIVATE_INLINE_HPP

namespace klib {

// Ugly to make this const, but it only modifies mutable members.
inline void string::changeAlloc( size_t size ) const {
	char* tmpPtr = new char[size];
	memcpy( tmpPtr, strPtr_, strSize_ );
	delete[] strPtr_;
	strPtr_ = tmpPtr;
}

inline size_t string::calcAllocSize( size_t size ) const {
	// Floors to roundto_, then adds roundto_.
	// If roundto_ is not a power of 2, does funny things. A half-minute of
	// thought tells me that it'd still give values greater than or equal
	// to size, but I'm not sure.
	return ( size & ~(roundto_ - 1) ) + roundto_;
}

} // namespace klib

#endif // PRIVATE_INLINE_HPP
