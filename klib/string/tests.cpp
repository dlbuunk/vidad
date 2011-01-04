//==---==  klib/string/tests.cpp  ==-------------------==  *- C++ -*  ==---==>
//
// This source file provides the tests for making sure klib::string works as
// expected. Any tests that passed during the previous commit must still pass
// after the next commit. Tests should check the output of the following
// functions:
//    const char* c_str() const; (not implemented)
//    bool empty() const; (not implemented)
//    size_t length() const; (not implemented)
//    size_t size() const; (not implemented)
//    size_t capacity() const; (not implemented)
//    bool operator==( string const& str ) const; (not implemented)
//    bool operator==( char const* cstrPtr ) const; (not implemented)
//
// If two functions share a name, only one needs to be included in the tests.
// The two operator[]s do not need to be included in every test, as it usually
// offers little above an std::strcmp, klib::string::strcmp (not implemented)
// or klib::string::operator==.
//
//
//==-----------------------------------------------------------------------==>
//#include <iostream>
#include <gtest/gtest.h>
#include <cstring>
#include "string.h"


TEST( klibstring, ConstructorDefault ) {
	// Testing: klib::string::string();
	// Ensure that the constructed string behaves as if it contains a
	// single \0, but no memory is allocated and instead the string is
	// faked, returning a reference to an internal character.
	klib::string s;
	EXPECT_EQ( s.size(), 1 );
	EXPECT_EQ( s.length(), 1 );
	EXPECT_EQ( s.capacity(), 0 );
	EXPECT_TRUE( s.empty() );
	EXPECT_EQ( std::strlen( s.c_str() ), 1 );
	EXPECT_TRUE( s == "" );
	EXPECT_STREQ( s.c_str(), "" );
	EXPECT_EQ( s[0], '\0' );
	s[0] = 'A'; // Due to the string being faked, writes are safe.
	EXPECT_EQ( s[0], '\0' ); // Making sure the string is unaffected.
}

TEST( klibstring, ConstructorFromString ) {
	// Testing: klib::string::string( klib::string const &str );
	// Ensure that the constructed string contains str. The length and size
	// of the two strings must be equal, and the strings pointed to must be
	// identical.
	char const *p = "Testing string.";
	klib::string t( p ); // Temporary string.
	klib::string s( tmp ); // String being tested. It is this one, not the
	                       // previous because it must be initialised from
	                       // a string.
	EXPECT_EQ( s.size(), t.size() );
	EXPECT_EQ( s.length(), t.length() );
	EXPECT_GE( s.size(), s.capacity() );
	EXPECT_FALSE( s.empty() );
	EXPECT_TRUE( s == t );
	EXPECT_STREQ( s.c_str(), t.c_str() );
}

TEST( klibstring, ConstructorFromCString ) {
	// Testing: klib::string& klib::string::string( char const *cstrPtr );
	// Ensure that the constructed string contains the C string pointed to
	// by strPtr. The size of the two strings must be equal, and the string
	// pointed to must contain the same value. The size may not be lower
	// than the capacity.
	{
		char const *p = "Testing string.";
		klib::string s( p );
		EXPECT_EQ( s.size(), std::strlen( p ) ); 
		EXPECT_EQ( s.length(), std::strlen( p ) );
		EXPECT_GE( s.size(), s.capacity() );
		EXPECT_FALSE( s.empty() );
		EXPECT_TRUE( s == p );
		EXPECT_STREQ( s.c_str(), p );
	}
	{
		char const *p = "Really somewhat longer than it needs to be \
but hey why not try it for the sake of making sure testing string.";
		klib::string s( p );
		EXPECT_EQ( s.size(), std::strlen( p ) ); 
		EXPECT_EQ( s.length(), std::strlen( p ) );
		EXPECT_GE( s.size(), s.capacity() );
		EXPECT_TRUE( s == p );
		EXPECT_FALSE( s.empty() );
		EXPECT_STREQ( s.c_str(), p );
	}
	{
		// klib::string may fake the memory allocation.
		char const *p = "";
		klib::string s( p );
		EXPECT_EQ( s.size(), std::strlen( p ) ); 
		EXPECT_EQ( s.length(), std::strlen( p ) );
		EXPECT_TRUE( s.empty() );
		EXPECT_TRUE( s == p );
		// Capacity may be anything.
		EXPECT_STREQ( s.c_str(), p );
	}
}

TEST( klibstring, AssignmentOperatorFromString ) {
	// Testing: klib::string& klib::string::operator=( klib::string const
	// &str );
	// Ensure that the new string is equal to the source string in content,
	// size and length, is only empty if the source string is empty, and
	// that its capacity is at least as large as its size (or the size is
	// equal to 1).
	{
		char const *p = "Testing string.";
		klib::string t( p );
		klib::string s = t;
		EXPECT_EQ( s.size(), t.size() );
		EXPECT_EQ( s.length(), t.length() );
		EXPECT_GE( s.size(), s.capacity() );
		EXPECT_FALSE( s.empty() );
		EXPECT_TRUE( s == t );
		EXPECT_STREQ( s.c_str(), t.c_str() );
	}
	{
		// klib::string may fake the memory allocation.
		char const *p = "";
		klib::string t( p );
		klib::string s = t;
		EXPECT_EQ( s.size(), 1 );
		EXPECT_EQ( s.length(), 1 );
		// Capacity may be anything.
		EXPECT_TRUE( s.empty() );
		EXPECT_TRUE( s == t );
		EXPECT_STREQ( s.c_str(), t.c_str() );
		EXPECT_EQ( s[0], '\0' );
	}
	{
		// Overwriting a long string with a short one.
		char const *p = "Other testing string.";
		klib::string t( p );
		klib::string s( "Testing string." )
		s = t;
		EXPECT_EQ( s.size(), t.size() );
		EXPECT_EQ( s.length(), t.length() );
		EXPECT_GE( s.size(), s.capacity() );
		EXPECT_FALSE( s.empty() );
		EXPECT_TRUE( s == t );
		EXPECT_STREQ( s.c_str(), t.c_str() );
	}
	{
		// Overwriting a short string with a long one.
		char const *p = "Testing string.";
		klib::string t( p );
		klib::string s( "Other testing string." )
		s = t;
		EXPECT_EQ( s.size(), t.size() );
		EXPECT_EQ( s.length(), t.length() );
		EXPECT_GE( s.size(), s.capacity() );
		EXPECT_FALSE( s.empty() );
		EXPECT_TRUE( s == t );
		EXPECT_STREQ( s.c_str(), t.c_str() );
	}
}

TEST( klibstring, AssignmentOperatorFromCString ) {
	// Testing: klib::string::operator=( char const *cstrPtr );
	// Ensure that the string stored is identical to the provided string,
	// and that the size and length are correct. Make sure whether it is
	// empty or not is shown correctly. 
	{
		char const *p = "Testing string.";
		klib::string s = *p;
		EXPECT_EQ( s.size(), std::strlen( p ) );
		EXPECT_EQ( s.length(), std::strlen( p ) );
		EXPECT_GE( s.size(), s.capacity() );
		EXPECT_FALSE( s.empty() );
		EXPECT_TRUE( s == p );
		EXPECT_STREQ( s.c_str(), p );
	}
	{
		// klib::string may fake the memory allocation.
		char const *p = "";
		klib::string s = p;
		EXPECT_EQ( s.size(), 1 );
		EXPECT_EQ( s.length(), 1 );
		// Capacity may be anything.
		EXPECT_TRUE( s.empty() );
		EXPECT_TRUE( s == p );
		EXPECT_STREQ( s.c_str(), p );
		EXPECT_EQ( s[0], '\0' );
	}
	{
		// Overwriting a long string with a short one.
		char const *p = "Other testing string.";
		klib::string s( "Testing string." )
		s = p;
		EXPECT_EQ( s.size(), std::strlen( p ) );
		EXPECT_EQ( s.length(), std::strlen( p ) );
		EXPECT_GE( s.size(), s.capacity() );
		EXPECT_FALSE( s.empty() );
		EXPECT_TRUE( s == p );
		EXPECT_STREQ( s.c_str(), p );
	}
	{
		// Overwriting a short string with a long one.
		char const *p = "Testing string.";
		klib::string s( "Other testing string." )
		s = p;
		EXPECT_EQ( s.size(), std::strlen( p ) );
		EXPECT_EQ( s.length(), std::strlen( p ) );
		EXPECT_GE( s.size(), s.capacity() );
		EXPECT_FALSE( s.empty() );
		EXPECT_TRUE( s == p );
		EXPECT_STREQ( s.c_str(), p );
	}
}

TEST( klibstring, AppendOperatorFromString ) {
	// Testing: string& operator+=( string const& str );
	// Ensure that the size and length of the string are correct, and
	// that C functions confirm that. Emptiness check should be correct.
	// Comparison should 

}

TEST( klibstring, AppendOperatorFromCString ) {

}

TEST( klibstring, AppendOperatorFromChar ) {

}

TEST( klibstring, AppendOperatorFromInt ) {

}

TEST( klibstring, AccessOperators ) {
}

TEST( klibstring, EqualityOperators ) {
}

TEST( klibstring, ConstFuncs ) {
}

TEST( klibstring, MutableFuncs ) {
}

TEST( klibstring, StaticFuncs ) {
}

GTEST_API_ int main( int argc, char **argv ) {
	testing::InitGoogleTest( &argc, argv );
	return RUN_ALL_TESTS();
}

