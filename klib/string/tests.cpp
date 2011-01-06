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
#include <gtest/gtest.h>
#include <cstring>
#include "string.h"

// |- Test: klib::string::string( size_t res = 0 ); ---------------------------|
TEST( klibstringConstructorDefault, NoReserve ) {
	// Ensure that the string is constructed and behaves like an empty
	// string, but no memory is allocated.
	klib::string s;
	EXPECT_EQ( s.size(), 1 ) <<
	    "String is of an unexpected size.\n";
	EXPECT_EQ( s.length(), 0 ) <<
	    "String is of an unexpected length\n";
	EXPECT_EQ( s.capacity(), 0 ) <<
	    "String is not faked.\n";
	EXPECT_TRUE( s.empty() ) <<
	    "String is not empty.\n";
	EXPECT_EQ( std::strlen( s.c_str() ), 0 ) <<
	    "std::strlen() evaluates the string length incorrectly.\n";
	EXPECT_TRUE( s == "" ) <<
	    "String does not consider itself equal to an empty string.\n";
	EXPECT_STREQ( s.c_str(), "" ) <<
	    "String does not seem to be equal to an empty string.\n";
	EXPECT_EQ( s[0], '\0' ) << 
	    "Accessing fails to return a null.\n";
}

TEST( klibstringConstructorDefault, WithReserve ) {
	// Ensure the string is constructed and behaves like an empty string,
	// and res bytes are reserved.
	klib::string s( 42 );
	EXPECT_EQ( s.size(), 1 ) <<
	    "String is of an unexpected size.\n";
	EXPECT_EQ( s.length(), 0 ) <<
	    "String is of an unexpected length\n";
	EXPECT_EQ( s.capacity(), 42 ) <<
	    "String is not faked.\n";
	EXPECT_TRUE( s.empty() ) <<
	    "String is not empty.\n";
	EXPECT_EQ( std::strlen( s.c_str() ), 0 ) <<
	    "std::strlen() evaluates the string length incorrectly.\n";
	EXPECT_TRUE( s == "" ) <<
	    "String does not consider itself equal to an empty string.\n";
	EXPECT_STREQ( s.c_str(), "" ) <<
	    "String does not seem to be equal to an empty string.\n";
	EXPECT_EQ( s[0], '\0' ) <<
	    "Accessing fails to return a null.\n";
}
// |- Done: klib::string::string( size_t res = 0 ); --------------------------|

#if 0 // No point, these have no chance of passing anyway. (continued later)

// |- Test: klib::string::string( klib::string const &str, size_t res = 0 ); -|
TEST( klibstringConstructorFromString, NoReserve ) {
	// Ensure that the constructed string contains str. The length and size
	// of the two strings must be equal, and the strings pointed to must be
	// identical.
	char const *p = "Testing string.";
	klib::string t( p ); // Temporary string.
	klib::string s( t ); // String being tested. It is this one, not the
	                     // previous because it must be initialised from
	                     // a string.
	EXPECT_EQ( s.size(), t.size() ) << "Size mismatch.\n";
	EXPECT_EQ( s.length(), t.length() ) << "Length mismatch.\n";
	EXPECT_GE( s.size(), s.capacity() ) << "Size exceeds capacity.\n";
	EXPECT_FALSE( s.empty() ) << "String is empty.\n";
	EXPECT_TRUE( s == t ) << "Strings are unequal.\n";
	EXPECT_STREQ( s.c_str(), t.c_str() ) << "C strings are unequal.\n";
}
// |- Done: klib::string::string( klib::string const &str, size_t res = 0 ); -|

#endif // 0

// |- Test: klib::string& klib::string::string( char const *cstrPtr, ---------|
// |-------   size_t num = 0 ); ----------------------------------------------|
// Ensure that the constructed string contains the first num characters
// of the C string pointed to by strPtr.
// If num is zero, or no num is specified, the string must be copied
// until a \0 byte is encountered. In that case, the size of the
// two strings must be equal, and the strings must contain the same
// values.
// If a non-zero num is specified and the C string is null-terminated,
// the size must equal num and the length must be one less.
// If a non-zero num is specified and the C string is not null-
// terminated, the size must be one greater than num, and the length
// must be equal to num.
// In any case, the size may not exceed the the capacity.
TEST( klibstringConstructorFromCString, AutoLengthNoReserve ) {
	char const* p = "Testing string.";
	klib::string s( p );
	ASSERT_EQ( s.size(), std::strlen( p ) + 1 ) <<
	    "Size mismatch.\n"; 
	EXPECT_EQ( s.length(), std::strlen( p ) ) <<
	    "Length mismatch.\n";
	EXPECT_GE( s.size(), s.capacity() ) <<
	    "Capacity is lower than size.\n";
	EXPECT_FALSE( s.empty() ) <<
	    "String is unexpectedly empty.\n";
	EXPECT_TRUE( s == p ) <<
	    "String does not consider itself equal with p.\n";
	EXPECT_STREQ( s.c_str(), p ) <<
	    "String does not compare as equal to p.\n";
}

TEST( klibstringConstructorFromCString, AutoLengthNoReserveLong ) {
	char const* p = "Really somewhat longer than it needs to be \
but hey why not try it for the sake of making sure testing string.";
	klib::string s( p );
	ASSERT_EQ( s.size(), std::strlen( p ) + 1 ) <<
	    "Size mismatch.\n"; 
	EXPECT_EQ( s.length(), std::strlen( p ) ) <<
	    "Length mismatch.\n";
	EXPECT_GE( s.size(), s.capacity() ) <<
	    "Capacity is lower than size.\n";
	EXPECT_FALSE( s.empty() ) <<
	    "String is unexpectedly empty.\n";
	EXPECT_TRUE( s == p ) <<
	    "String does not consider itself equal with p.\n";
	EXPECT_STREQ( s.c_str(), p ) <<
	    "String does not compare as equal to p.\n";
}

TEST( klibstringConstructorFromCString, AutoLengthNoReserveEmpty ) {
	// klib::string may fake the memory allocation.
	char const* p = "";
	klib::string s( p );
	ASSERT_EQ( s.size(), 1 ) <<
	    "Size mismatch.\n"; 
	EXPECT_EQ( s.length(), 0 ) <<
	    "Length mismatch.\n";
	EXPECT_TRUE( s.empty() ) <<
	    "String unexpectedly non-empty.\n";
	EXPECT_TRUE( s == "" ) <<
	    "String does not consider itself equal to an empty \
string.\n";
	// Capacity may be anything.
	EXPECT_STREQ( s.c_str(), "" ) <<
	    "String does not compare as equal to an empty string.\n";
	EXPECT_EQ( s[0], '\0' ) <<
	    "First character of string is not a \\0.\n";
}

TEST( klibstringConstructorFromCString, ManualLengthNoReserve ) {
	char const* p = "Testing string.";
	klib::string s( p, 7 );
	ASSERT_EQ( s.size(), 8 ) <<
	    "Size mismatch.\n"; 
	EXPECT_EQ( s.length(), 7 ) <<
	    "Length mismatch.\n";
	EXPECT_GE( s.size(), s.capacity() ) <<
	    "Capacity is lower than size.\n";
	EXPECT_FALSE( s.empty() ) <<
	    "String is unexpectedly empty.\n";
	EXPECT_TRUE( s == "Testing" ) <<
	    "String comparison incorrect.\n";
	EXPECT_STREQ( s.c_str(), "Testing" ) <<
	    "String comparison incorrect.\n";
}

TEST( klibstringConstructorFromCString, ManualLengthNoReserve2 ) {
	char const* p = "Testing";
	klib::string s( p, 7 );
	ASSERT_EQ( s.size(), std::strlen( p ) + 1 ) <<
	    "Size mismatch.\n"; 
	EXPECT_EQ( s.length(), std::strlen( p ) ) <<
	    "Length mismatch.\n";
	EXPECT_GE( s.size(), s.capacity() ) <<
	    "Capacity is lower than size.\n";
	EXPECT_FALSE( s.empty() ) <<
	    "String is unexpectedly empty.\n";
	EXPECT_TRUE( s == p ) <<
	    "String does not consider itself equal with p.\n";
	EXPECT_STREQ( s.c_str(), p ) <<
	    "String does not compare as equal to p.\n";
}

TEST( klibstringConstructorFromCString, ExplicitAutoLengthNoReserve ) {
	char const* p = "Testing string.";
	klib::string s( p, 0 );
	ASSERT_EQ( s.size(), std::strlen( p ) + 1 ) <<
	    "Size mismatch.\n"; 
	EXPECT_EQ( s.length(), std::strlen( p ) ) <<
	    "Length mismatch.\n";
	EXPECT_GE( s.size(), s.capacity() ) <<
	    "Capacity is lower than size.\n";
	EXPECT_FALSE( s.empty() ) <<
	    "String is unexpectedly empty.\n";
	EXPECT_TRUE( s == p ) <<
	    "String does not consider itself equal with p.\n";
	EXPECT_STREQ( s.c_str(), p ) <<
		    "String does not compare as equal to p.\n";
}

TEST( klibstringConstructorFromCString, ExplicitAutoLengthWithReserve ) {
	char const* p = "Testing string.";
	klib::string s( p, 0, 321 );
	ASSERT_EQ( s.size(), std::strlen( p ) + 1 ) <<
	    "Size mismatch.\n"; 
	EXPECT_EQ( s.length(), std::strlen( p ) ) <<
	    "Length mismatch.\n";
	EXPECT_EQ( s.capacity(), 321 ) <<
	    "Capacity is not what was requested.\n";
	EXPECT_FALSE( s.empty() ) <<
	    "String is unexpectedly empty.\n";
	EXPECT_TRUE( s == p ) <<
	    "String does not consider itself equal with p.\n";
	EXPECT_STREQ( s.c_str(), p ) <<
	    "String does not compare as equal to p.\n";
}
// |- Done: klib::string& klib::string::string( char const *cstrPtr, ---------|
// |-------   size_t num = 0 ); ----------------------------------------------|

#if 0

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
		EXPECT_EQ( s.length(), 0 );
		// Capacity may be anything.
		EXPECT_TRUE( s.empty() );
		EXPECT_TRUE( s == t );
		EXPECT_STREQ( s.c_str(), t.c_str() );
		EXPECT_EQ( s[0], '\0' );
	}
	{
		// Overwriting a long string with a short one.
		char const *p = "Other testing string that is probably long \
enough to have more memory allocated for it.";
		klib::string t( p );
		klib::string s( "Testing string." );
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
		klib::string s( "Other testing string that is probably long \
enough to have more memory allocated for it." );
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
		EXPECT_EQ( s.length(), std::strlen( p ) - 1 );
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
		EXPECT_EQ( s.length(), 0 );
		// Capacity may be anything.
		EXPECT_TRUE( s.empty() );
		EXPECT_TRUE( s == p );
		EXPECT_STREQ( s.c_str(), p );
		EXPECT_EQ( s[0], '\0' );
	}
	{
		// Overwriting a long string with a short one.
		char const *p = "Other testing string that is probably long \
enough to have more memory allocated for it.";
		klib::string s( "Testing string." );
		s = p;
		EXPECT_EQ( s.size(), std::strlen( p ) );
		EXPECT_EQ( s.length(), std::strlen( p ) - 1 );
		EXPECT_GE( s.size(), s.capacity() );
		EXPECT_FALSE( s.empty() );
		EXPECT_TRUE( s == p );
		EXPECT_STREQ( s.c_str(), p );
	}
	{
		// Overwriting a short string with a long one.
		char const *p = "Testing string.";
		klib::string s( "Other testing string that is probably long \
enough to have more memory allocated for it." );
		s = p;
		EXPECT_EQ( s.size(), std::strlen( p ) );
		EXPECT_EQ( s.length(), std::strlen( p ) - 1 );
		EXPECT_GE( s.size(), s.capacity() );
		EXPECT_FALSE( s.empty() );
		EXPECT_TRUE( s == p );
		EXPECT_STREQ( s.c_str(), p );
	}
}

TEST( klibstring, IndexOperatorRead ) {
	// Testing: char const& operator[]( size_t pos ) const;
	// Ensure that the characters read are the ones expected.
	char const* p = "Test.";
	klib::string s( p );
	EXPECT_EQ( s[0], p[0] ) << "Inequal at index 0.\n";
	EXPECT_EQ( s[3], p[3] ) << "Inequal at index 3.\n";
	EXPECT_EQ( s[5], p[5] ) << "Inequal at index 5.\n";
	EXPECT_EQ( s[7], '\0' ) << "Out-of-bounds access did not return 0.\n";
}

TEST( klibstring, IndexOperatorWrite ) {
	// Testing: char& operator[]( size_t pos );
	// Ensure that the characters written are stored.
	// When used this way, this function does not modify the string as a
	// whole, and thus tests on length may be skipped. See
	// IndexOperatorSpecialWrite for cases where they do directly modify
	// it.
	klib::string s( "TTest." );
	s[0] = 'Q';
	EXPECT_EQ( s.c_str()[0], 'Q' ) << "Inequal at index 0.\n";
	s[1] = 'u';
	EXPECT_EQ( s.c_str()[1], 'u' ) << "Inequal at index 1.\n";
	s[5] = '!';
	EXPECT_EQ( s.c_str()[5], '!' ) << "Inequal at index 5.\n";
}

TEST( klibstring, IndexOperatorSpecialWrite ) {
	// Testing: char& operator[]( size_t pos );
	// Ensure that the string resizes correctly when the last element is
	// accessed, and that a string is allocated if element 0 is accessed
	// before it is created.
	klib::string s;
	s[0] = 'A';
	EXPECT_EQ( s.size(), 2 ) << "Size mismatch.\n";
	EXPECT_EQ( s.length(), 1 ) << "Length mismatch.\n";
	EXPECT_EQ( s[0], 'A' ) << "Value at index 0 set incorrectly.\n";
	s[1] = ' ';
	EXPECT_EQ( s.size(), 3 ) << "Size mismatch.\n";
	EXPECT_EQ( s.length(), 2 ) << "Length mismatch.\n";
	EXPECT_EQ( s[1], ' ' ) << "Value at index 1 set incorrectly.\n";
	s[2] = 'T';
	EXPECT_EQ( s.size(), 4 ) << "Size mismatch.\n";
	EXPECT_EQ( s.length(), 3 ) << "Length mismatch.\n";
	EXPECT_EQ( s[2], 'T' ) << "Value at index 2 set incorrectly.\n";
	EXPECT_TRUE( s == "A T" ) << "Strings are unequal for  ==.\n";
	EXPECT_STREQ( s.c_str(), "A T" ) << "Strings are unequal for STREQ.\n";
}

#endif // 0

// |- Test: bool operator==( char const* cstrPtr ) const; --------------------|
TEST( klibstringEqualityOperatorCString, Normal ) {
	// Ensure that a string with contents does compare as equal to itself,
	// but does not compare as equal to an empty string, or a different
	// string (no matter whether the length is the same or not).
	klib::string s( "Test." );
	EXPECT_TRUE( s == "Test." );
	EXPECT_FALSE( s == "" );
	EXPECT_FALSE( s == "Cake." );
	EXPECT_FALSE( s == "Other test." );
}

TEST( klibstringEqualityOperatorCString, Unallocated ) {
	// Ensure that an unallocated string compares as equal to an empty
	// string, and as unequal to a non-empty string.
	klib::string s;
	EXPECT_TRUE( s == "" );
	EXPECT_FALSE( s == "Test." );
}
// |- Done: bool operator==( char const* cstrPtr ) const; --------------------|

// |- Test: bool empty() const; ----------------------------------------------|
TEST( klibstringEmpty, DefaultConstructor ) {
	// Ensure that a string created with the default constructor is empty.
	klib::string s;
	EXPECT_TRUE( s.empty() );
}

TEST( klibstringEmpty, EmptyCopy ) {
	// Ensure that a string copied from an empty string shows up as empty.
	klib::string s( "" );
	EXPECT_TRUE( s.empty() );
}

TEST( klibstringEmpty, EmptyAssigned ) {
	// Ensure that a string that an empty string is assigned to shows up as
	// empty.
	klib::string s;
	s = "";
	EXPECT_TRUE( s.empty() );
}

TEST( klibstringEmpty, EmptyAfterNullAppend ) {
	// Ensure that even if a null is appended, an empty string stays empty
	// (make sure the append actually works, too).
	klib::string s( "" );
	s[0] = '\0'; // This writes the last character, and thus
		     // an \0 is appended.
	EXPECT_EQ( s.size(), 2 ); // If false, the append failed.
	EXPECT_TRUE( s.empty() );

}

TEST( klibstringEmpty, NotEmpty ) {
	// Ensure that if it is given a non-empty string, it will show up as
	// not empty.
	klib::string s( "Testing string" );
	EXPECT_FALSE( s.empty() );
}

TEST( klibstringEmpty, SetFirstCharToNull ) {
	char const* p = "Test."; // Six chars long
	klib::string s( p );
	s[0] = '\0'; // The string size is unaffected, although
		     // strlen would now return 0.
	EXPECT_EQ( s.size(), 6 ) << // No change expected.
	    "Size mismatch after setting first byte to null.\n"; 
	EXPECT_FALSE( s.empty() ) << // No change expected.
	    "String appears as empty after setting first byte to null.\n";
}
// |- Done: bool empty() const; ----------------------------------------------|

// |- Test: size_t size() const; ---------------------------------------------|
TEST( klibstringSize, Normal ) {
	// Ensure that the size returned is equal to the size of the string.
	char const* p = "Testing string.";
	klib::string s( p );
	EXPECT_EQ( s.size(), std::strlen( p ) + 1 );
}

TEST( klibstringSize, Empty ) {
	// Ensure that the size of an unallocated empty string is equal to 1.
	klib::string s;
	EXPECT_EQ( s.size(), 1 ) <<
	    "Size mismatch.\n";
}
// |- Test: size_t size() const; ----------------------------------------------|

// |- Test: size_t length() const; --------------------------------------------|
TEST( klibstringLength, Normal ) {
	// Ensure that the length returned is equal to the length of the string.
	char const* p = "Testing string.";
	klib::string s( p );
	EXPECT_EQ( s.length(), std::strlen( p ) );
}

TEST( klibstringLength, Empty ) {
	// Ensure that an unallocated empty string has a length of 0.
	klib::string s;
	EXPECT_EQ( s.length(), 0 );
}
// |- Test: size_t length() const; --------------------------------------------|

// |- Test: size_t capacity() const; ------------------------------------------|
// |- Due to relying on internal workings of the class, these tests should be -|
// |- written in a way that will not start failing when behaviour is changed. -|
// |- Thus, it is advised to only check for cases where memory reservation is -|
// |- explicitly requested AND possible. --------------------------------------|
TEST( klibstringCapacity, DefaultConstructorNoReserve ) {
	// Ensure that an empty string, constructed with no request to reserve
	// memory, has a capacity of 0.
	klib::string s;
	EXPECT_EQ( s.capacity(), 0 ) <<
	    "Capacity set incorrectly.\n";
}

TEST( klibstringCapacity, DefaultConstructorWithReserve ) {
	// Ensure that an empty string, constructed with a request to reserve
	// memory, has the reserved amount.
	klib::string s( 16 );
	EXPECT_EQ( s.capacity(), 16 ) <<
	    "Capacity set incorrectly.\n";
}

TEST( klibstringCapacity, StringConstructorWithReserve ) {
	// Ensure that a string created from a C string reserves the requested
	// amount of memory if a request is made.
	char const* p = "Testing string.";
	klib::string t( p );
	klib::string s( t, 31 );
	EXPECT_EQ( s.capacity(), 31 ) <<
	    "Capacity set incorrectly.\n";
	EXPECT_GE( s.capacity(), s.size() ) <<
	    "Size exceeds capacity.\n";
}

TEST( klibstringCapacity, CStringConstructorWithReserve ) {
	// Ensure that a string created from a C string reserves the requested
	// amount of memory if a request is made.
	char const* p = "Testing string.";
	klib::string s( p, 0, 31 );
	EXPECT_EQ( s.capacity(), 31 ) <<
	    "Capacity set incorrectly.\n";
	EXPECT_GE( s.capacity(), s.size() ) <<
	    "Size exceeds capacity.\n";
}
// |- Done: size_t capacity() const; ------------------------------------------|

// |- Test: static size_t strlen( char const* cstr ); -------------------------|
TEST( klibstringCStringLength, Normal ) {
	// Ensure that if the string contains no \0, the number of chars is
	// returned.
	EXPECT_EQ( klib::string::strlen( "Test." ), 5 );
}

TEST( klibstringCStringLength, Empty ) {
	// Ensure that if the string is empty, 0 is returned.
	EXPECT_EQ( klib::string::strlen( "" ), 0 );
}

TEST( klibstringCStringLength, EarlyNull ) {
	// Ensure that if the string has an early \0, the chars after it are
	// not counted.
	EXPECT_EQ( klib::string::strlen( "\0Test." ), 0 );
}
// |- Done: static size_t strlen( char const* cstr ); -------------------------|

// |- Test: static int strcmp( char const* cstrA, char const* cstrB ); --------|
TEST( klibstringCStringCompare, Equal ) {
	// Ensure that if two identical C strings are inputted, 0 is returned.
	EXPECT_EQ( klib::string::strcmp( "Test.", "Test." ), 0 ) <<
	    "Strings do not compare as equal.\n";
}

TEST( klibstringCStringCompare, FirstGreater ) {
	// Ensure that if the first character that differs is greater in the 
	// first string, a positive value is returned.
	EXPECT_LT( klib::string::strcmp( "Test.", "Westz." ), 0 ) <<
	    "First string does not compare as greater than second.\n";
}

TEST( klibstringCStringCompare, SecondGreater ) {
	// Ensure that if the first character that differs is greater in the
	// second string, a negative value is returned.
	EXPECT_GT( klib::string::strcmp( "Test.", "Pesta." ), 0 ) <<
	    "Second string does not compare as greater than first.\n";
}

TEST( klibstringCStringCompare, BothEmpty ) {
	// Ensure that if both strings are empty, they compare as equal.
	EXPECT_EQ( klib::string::strcmp( "", "" ), 0 ) <<
	    "Two empty strings compare as unequal.\n";
}

TEST( klibstringCStringCompare, BothNullThenNonsense ) {
	// Ensure that if a string is a \0 followed by something, it will
	// compare as equal to a single \0.
	EXPECT_EQ( klib::string::strcmp( "\0Nonsense", "" ), 0 ) <<
	    "A null and an empty string compare as unequal.\n";
}

TEST( klibstringCStringCompare, EarlyNullTermination ) {
	// Ensure that if one string is null-terminated early, it will compare
	// to another string that does not contain the text after the 
	// null-termination.
	EXPECT_EQ( klib::string::strcmp( "Test\0ing", "Test" ), 0 ) <<
	    "A null and an empty string compare as unequal.\n";
}
// |- Done: static int strcmp( char const* cstrA, char const* cstrB ); -------|


GTEST_API_ int main( int argc, char **argv ) {
	testing::InitGoogleTest( &argc, argv );
	return RUN_ALL_TESTS();
}

