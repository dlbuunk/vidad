//==---==  klib/cstring/tests.cpp  ==--------------------==  *- C++ -*  ==---==>
//
// This source file provides the tests for making sure that the functions in
// cstring (header file) work as expected.
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
#include <gtest/gtest.h>
#include "cstring.h.gen"

// |- Test: size_t strlen( char const* cstr ); --------------------------------|
TEST( klibcstringLength, Normal ) {
	// Ensure that if the string contains no \0, the number of chars is
	// returned.
	EXPECT_EQ( klib::strlen( "Test." ), 5 );
}

TEST( klibcstringLength, Empty ) {
	// Ensure that if the string is empty, 0 is returned.
	EXPECT_EQ( klib::strlen( "" ), 0 );
}

TEST( klibcstringLength, EarlyNull ) {
	// Ensure that if the string has an early \0, the chars after it are
	// not counted.
	EXPECT_EQ( klib::strlen( "\0Test." ), 0 );
}
// |- Done: size_t strlen( char const* cstr ); --------------------------------|

// |- Test: int strcmp( char const* cstrA, char const* cstrB ); ---------------|
TEST( klibcstringCompare, Equal ) {
	// Ensure that if two identical C strings are inputted, 0 is returned.
	EXPECT_EQ( klib::strcmp( "Test.", "Test." ), 0 ) <<
	    "Strings do not compare as equal.\n";
}

TEST( klibcstringCompare, FirstGreater ) {
	// Ensure that if the first character that differs is greater in the 
	// first string, a positive value is returned.
	EXPECT_LT( klib::strcmp( "Test.", "Westz." ), 0 ) <<
	    "First string does not compare as greater than second.\n";
}

TEST( klibcstringCompare, SecondGreater ) {
	// Ensure that if the first character that differs is greater in the
	// second string, a negative value is returned.
	EXPECT_GT( klib::strcmp( "Test.", "Pesta." ), 0 ) <<
	    "Second string does not compare as greater than first.\n";
}

TEST( klibcstringCompare, BothEmpty ) {
	// Ensure that if both strings are empty, they compare as equal.
	EXPECT_EQ( klib::strcmp( "", "" ), 0 ) <<
	    "Two empty strings compare as unequal.\n";
}

TEST( klibcstringCompare, BothNullThenNonsense ) {
	// Ensure that if a string is a \0 followed by something, it will
	// compare as equal to a single \0.
	EXPECT_EQ( klib::strcmp( "\0Nonsense", "" ), 0 ) <<
	    "A null and an empty string compare as unequal.\n";
}

TEST( klibcstringCompare, EarlyNullTermination ) {
	// Ensure that if one string is null-terminated early, it will compare
	// to another string that does not contain the text after the 
	// null-termination.
	EXPECT_EQ( klib::strcmp( "Test\0ing", "Test" ), 0 ) <<
	    "A null and an empty string compare as unequal.\n";
}
// |- Done: int strcmp( char const* cstrA, char const* cstrB ); ---------------|

// |- Test: char* strcpy( char* dest, char const* src -------------------------|
TEST( klibcstringStringCopy, Normal ) {
	// Ensure that a normal string gets copied correctly.
	char src[10] = "Test.";
	char dest[10];
	klib::strcpy( dest, src );
	EXPECT_STREQ( dest, src );
}

TEST( klibcstringStringCopy, Empty ) {
	// Ensure that a normal string gets copied correctly.
	char src[10] = "";
	char dest[10];
	klib::strcpy( dest, src );
	EXPECT_STREQ( dest, src );
}

TEST( klibcstringStringCopy, EarlyEnd ) {
	// Ensure that a normal string gets copied correctly.
	char src[32] = "Test.\0More.";
	char dest[32];
	klib::strcpy( dest, src );
	EXPECT_STREQ( dest, src );
}
// |- Done: char* klib::strcpy( char* dest, char const* src -------------------|

// |- Test: void* klib::memcpy( void* dest, void const* src, size_t num ); ----|
TEST( klibcstringMemCopy, Char ) {
	// Ensure that a single character is copied correctly, and that the
	// address returned is equal to the address of the destination.
	char src = 'A';
	char dest;
	char* check = (char*) klib::memcpy( &dest, &src, 1 );
	EXPECT_EQ( dest, 'A' ) << "Copy failed.\n";
	EXPECT_EQ( &dest, check ) << "Return value incorrect.\n";
}

TEST( klibcstringMemCopy, UnsignedShort ) {
	// Ensure that a single word is copied correctly, and that the
	// address returned is equal to the address of the destination.
	word src = 0xDDD;
	word dest;
	word* check = (word*)klib::memcpy( &dest, &src, 2 );
	EXPECT_EQ( dest, 0xDDD ) << "Copy failed.\n";
	EXPECT_EQ( &dest, check ) << "Return value incorrect.\n";
}

TEST( klibcstringMemCopy, UnsignedInt ) {
	// Ensure that a single dword is copied correctly, and that the
	// address returned is equal to the address of the destination.
	dword src = 0xFFFFFFFF;
	dword dest;
	dword* check = (dword*) klib::memcpy( &dest, &src, 4 );
	EXPECT_EQ( dest, 0xFFFFFFFF ) << "Copy failed.\n";
	EXPECT_EQ( &dest, check ) << "Return value incorrect.\n";
}

TEST( klibcstringMemCopy, IntArray ) {
	// Ensure that the arrays of ints are copied correctly, and that the
	// address returned is equal to the address of the destination.
	dword src[4] = { 0xFFFFFFFF, 0, 0xDD, 0x10 };
	dword dest[4];
	dword* check = (dword*) klib::memcpy( dest, src, 16 );
	EXPECT_EQ( dest[0], 0xFFFFFFFF ) << "Copy failed.\n";
	EXPECT_EQ( dest[1], 0 ) << "Copy failed.\n";
	EXPECT_EQ( dest[2], 0xDD ) << "Copy failed.\n";
	EXPECT_EQ( dest[3], 0x10 ) << "Copy failed.\n";
	EXPECT_EQ( dest, check ) << "Return value incorrect.\n";
}
// |- Done: void* klib::memcpy( void* dest, void const* src, size_t num ); ----|

// |- Test: void* klib::memmove( void* dest, void const* src, size_t num ); ---|
TEST( klibcstringMemMove, SeparateArrays ) {
	// Ensure that moving four bytes one backwards one byte works.
	byte src[8] = { 0x88, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11 };
	byte dest[8];
	byte* check = (byte*) klib::memmove( dest, src, 8 );
	EXPECT_EQ( (dword) dest[0], 0x88 ) << "Value incorrect.\n";
	EXPECT_EQ( (dword) dest[1], 0x77 ) << "Value incorrect.\n";
	EXPECT_EQ( (dword) dest[2], 0x66 ) << "Value incorrect.\n";
	EXPECT_EQ( (dword) dest[3], 0x55 ) << "Value incorrect.\n";
	EXPECT_EQ( (dword) dest[4], 0x44 ) << "Value incorrect.\n";
	EXPECT_EQ( (dword) dest[5], 0x33 ) << "Value incorrect.\n";
	EXPECT_EQ( (dword) dest[6], 0x22 ) << "Value incorrect.\n";
	EXPECT_EQ( (dword) dest[7], 0x11 ) << "Value incorrect.\n";
	EXPECT_EQ( dest, check ) << "Return value incorrect.\n";
}

TEST( klibcstringMemMove, FourBytesTwoUp ) {
	// Ensure that moving four bytes one backwards one byte works.
	byte arr[8] = { 0x88, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11 };
	byte* check = (byte*) klib::memmove( arr+2, arr, 4 );
	EXPECT_EQ( (dword) arr[0], 0x88 ) << "Value changed.\n";
	EXPECT_EQ( (dword) arr[1], 0x77 ) << "Value changed.\n";
	EXPECT_EQ( (dword) arr[2], 0x88 ) << "Value incorrect.\n";
	EXPECT_EQ( (dword) arr[3], 0x77 ) << "Value incorrect.\n";
	EXPECT_EQ( (dword) arr[4], 0x66 ) << "Value incorrect.\n";
	EXPECT_EQ( (dword) arr[5], 0x55 ) << "Value incorrect.\n";
	EXPECT_EQ( (dword) arr[6], 0x22 ) << "Value changed.\n";
	EXPECT_EQ( (dword) arr[7], 0x11 ) << "Value changed.\n";
	EXPECT_EQ( arr+2, check ) << "Return value incorrect.\n";
}

TEST( klibcstringMemMove, FourBytesTwoDown ) {
	// Ensure that moving four bytes down two bytes works.
	byte arr[8] = { 0x88, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11 };
	byte* check = (byte*) klib::memmove( arr, arr+2, 4 );
	EXPECT_EQ( (dword) arr[0], 0x66 ) << "Value changed.\n";
	EXPECT_EQ( (dword) arr[1], 0x55 ) << "Value changed.\n";
	EXPECT_EQ( (dword) arr[2], 0x44 ) << "Value incorrect.\n";
	EXPECT_EQ( (dword) arr[3], 0x33 ) << "Value incorrect.\n";
	EXPECT_EQ( (dword) arr[4], 0x44 ) << "Value incorrect.\n";
	EXPECT_EQ( (dword) arr[5], 0x33 ) << "Value incorrect.\n";
	EXPECT_EQ( (dword) arr[6], 0x22 ) << "Value changed.\n";
	EXPECT_EQ( (dword) arr[7], 0x11 ) << "Value changed.\n";
	EXPECT_EQ( arr, check ) << "Return value incorrect.\n";
}

TEST( klibcstringMemMove, FourDWordsTwoDown ) {
	// Ensure that moving four dwords down two dwords works.
	dword arr[8] = { 0x88888888, 0x77777777, 0x66666666, 0x55555555,
	                 0x44444444, 0x33333333, 0x22222222, 0x11111111 };
	dword* check = (dword*) klib::memmove( arr, arr+2, 16 );
	EXPECT_EQ( arr[0], 0x66666666 ) << "Value changed.\n";
	EXPECT_EQ( arr[1], 0x55555555 ) << "Value changed.\n";
	EXPECT_EQ( arr[2], 0x44444444 ) << "Value incorrect.\n";
	EXPECT_EQ( arr[3], 0x33333333 ) << "Value incorrect.\n";
	EXPECT_EQ( arr[4], 0x44444444 ) << "Value incorrect.\n";
	EXPECT_EQ( arr[5], 0x33333333 ) << "Value incorrect.\n";
	EXPECT_EQ( arr[6], 0x22222222 ) << "Value changed.\n";
	EXPECT_EQ( arr[7], 0x11111111 ) << "Value changed.\n";
	EXPECT_EQ( arr, check ) << "Return value incorrect.\n";
}
// |- Done: void* klib::memmove( void* dest, void const* src, size_t num ); ---|

// |- Test: int memcmp( void const* cstrA, void const* cstrB, size_t num ); ---|
TEST( klibcstringMemCompare, Equal ) {
	// Ensure that if two identical C strings are inputted, 0 is returned.
	EXPECT_EQ( klib::memcmp( "Test.", "Test.", 5 ), 0 ) <<
	    "Strings do not compare as equal.\n";
}

TEST( klibcstringMemCompare, FirstGreater ) {
	// Ensure that if the first character that differs is greater in the 
	// first string, a positive value is returned.
	EXPECT_LT( klib::memcmp( "Test.", "Westz.", 5 ), 0 ) <<
	    "First string does not compare as greater than second.\n";
}

TEST( klibcstringMemCompare, SecondGreater ) {
	// Ensure that if the first character that differs is greater in the
	// second string, a negative value is returned.
	EXPECT_GT( klib::memcmp( "Test.", "Pesta.", 5 ), 0 ) <<
	    "Second string does not compare as greater than first.\n";
}

TEST( klibcstringMemCompare, BothEmpty ) {
	// Ensure that if both strings are empty, they compare as equal.
	EXPECT_EQ( klib::memcmp( "", "", 1 ), 0 ) <<
	    "Two empty strings compare as unequal.\n";
}

TEST( klibcstringMemCompare, BothNullThenNonsense ) {
	// Ensure that if a string is a \0 followed by something, it will
	// compare as equal to a single \0.
	EXPECT_LE( klib::memcmp( "\0Nonsense", "Nonsense", 8 ), 0 ) <<
	    "A null and an empty string compare as unequal.\n";
}

TEST( klibcstringMemCompare, EarlyNullTermination ) {
	// Ensure that if one string is null-terminated early, it will compare
	// to another string that does not contain the text after the 
	// null-termination.
	EXPECT_EQ( klib::memcmp( "Test\0ing", "Test\0ing", 8 ), 0 ) <<
	    "A null and an empty string compare as unequal.\n";
}
// |- Done: int memcmp( char const* cstrA, char const* cstrB ); ---------------|

GTEST_API_ int main( int argc, char **argv ) {
	testing::InitGoogleTest( &argc, argv );
	return RUN_ALL_TESTS();
}

