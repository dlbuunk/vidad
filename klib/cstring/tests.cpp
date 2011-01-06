//==---==  klib/cstring/tests.cpp  ==--------------------==  *- C++ -*  ==---==>
//
// This source file provides the tests for making sure that the functions in
// cstring (header file) work as expected.
//
//==-------------------------------------------------------------------------==>
#include <gtest/gtest.h>
#include "cstring.h"

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

// |- Test: void* memcpy( void* dest, void const* src, size_t num ); ----------|
TEST( klibcstringMemCopy, Char ) {
	// Ensure that a single character is copied correctly, and that the
	// address returned is equal to the address of the destination.
	char src = 'A';
	char dest;
	char* check = (char*) memcpy( &dest, &src, 1 );
	EXPECT_EQ( dest, 'A' ) << "Copy failed.\n";
	EXPECT_EQ( &dest, check ) << "Return value incorrect.\n";
}

TEST( klibcstringMemCopy, UnsignedShort ) {
	// Ensure that a single unsigned short is copied correctly, and that the
	// address returned is equal to the address of the destination.
	unsigned short src = 0xDDD;
	unsigned short dest;
	unsigned short* check = (unsigned short*) memcpy( &dest, &src, 2 );
	EXPECT_EQ( dest, 0xDDD ) << "Copy failed.\n";
	EXPECT_EQ( &dest, check ) << "Return value incorrect.\n";
}

TEST( klibcstringMemCopy, UnsignedInt ) {
	// Ensure that a single unsigned int is copied correctly, and that the
	// address returned is equal to the address of the destination.
	unsigned int src = 0xFFFFFFFF;
	unsigned int dest;
	unsigned int* check = (unsigned int*) memcpy( &dest, &src, 4 );
	EXPECT_EQ( dest, 0xFFFFFFFF ) << "Copy failed.\n";
	EXPECT_EQ( &dest, check ) << "Return value incorrect.\n";
}

TEST( klibcstringMemCopy, IntArray ) {
	// Ensure that the arrays of ints are copied correctly, and that the
	// address returned is equal to the address of the destination.
	unsigned int src[4] = { 0xFFFFFFFF, 0, 0xDD, 0x10 };
	unsigned int dest[4];
	unsigned int* check = (unsigned int*) memcpy( dest, src, 16 );
	EXPECT_EQ( dest[0], 0xFFFFFFFF ) << "Copy failed.\n";
	EXPECT_EQ( dest[1], 0 ) << "Copy failed.\n";
	EXPECT_EQ( dest[2], 0xDD ) << "Copy failed.\n";
	EXPECT_EQ( dest[3], 0x10 ) << "Copy failed.\n";
	EXPECT_EQ( dest, check ) << "Return value incorrect.\n";
}
// |- Done: void* memcpy( void* dest, void const* src, size_t num ); ----------|

GTEST_API_ int main( int argc, char **argv ) {
	testing::InitGoogleTest( &argc, argv );
	return RUN_ALL_TESTS();
}

