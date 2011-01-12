//==---==  klib/cstddef/cstddef.hpp ==-------------------==  *- C++ -*  ==---==>
//
//! \file klib/cstddef/cstddef.hpp
//! \brief Mostly standard-compliant header with typedefs and defines.
//!
//! Apart from size_t and NULL, this header defines the different integer sizes
//! available on x86.
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
#ifndef CSTDDEF_HPP
#define CSTDDEF_HPP

//! Unsigned type to be used for memory sizes.
typedef unsigned int size_t;
//! 8-bit unsigned integer.
typedef unsigned char byte;
//! 16-bit unsigned integer.
typedef unsigned short word;
//! 32-bit unsigned integer.
typedef unsigned int dword;
//! 64-bit unsigned integer.
typedef unsigned long long qword;
#ifdef __cplusplus
	//! C++-only define to allow NULL to be treated correctly.
	#define NULL 0
#endif

#endif // CSTDDEF_HPP
