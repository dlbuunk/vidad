//==---==  klib/string/tests.cpp  ==---------------------==  *- C++ -*  ==---==>
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
//==-------------------------------------------------------------------------==>
#include <gtest/gtest.h>
#include <cstring>
#include "string.h.gen"

// |- Test: string( size_t res = 0 ); -----------------------------------------|
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

TEST( klibstringConstructorDefault, NoReserveDynamic ) {
	// Ensure that the string is constructed and behaves like an empty
	// string, but no memory is allocated.
	klib::string *s = new klib::string;
	EXPECT_EQ( s->size(), 1 ) <<
	    "String is of an unexpected size.\n";
	EXPECT_EQ( s->length(), 0 ) <<
	    "String is of an unexpected length\n";
	EXPECT_EQ( s->capacity(), 0 ) <<
	    "String is not faked.\n";
	EXPECT_TRUE( s->empty() ) <<
	    "String is not empty.\n";
	EXPECT_EQ( std::strlen( s->c_str() ), 0 ) <<
	    "std::strlen() evaluates the string length incorrectly.\n";
	EXPECT_TRUE( *s == "" ) <<
	    "String does not consider itself equal to an empty string.\n";
	EXPECT_STREQ( s->c_str(), "" ) <<
	    "String does not seem to be equal to an empty string.\n";
	EXPECT_EQ( (*s)[0], '\0' ) << 
	    "Accessing fails to return a null.\n";
	delete s;
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
// |- Done: string( size_t res = 0 ); ----------------------------------------|

// |- Test: string( string const &str, size_t res = 0 ); ---------------------|
TEST( klibstringConstructorFromString, NoReserve ) {
	// Ensure that the constructed string contains str. 
	char const *p = "Testing string.";
	klib::string t( p ); // Temporary string.
	klib::string s( t ); // String being tested. It is this one, not the
	                     // previous because it must be initialised from
	                     // a string.
	EXPECT_EQ( s.size(), t.size() ) << "Size mismatch.\n";
	EXPECT_EQ( s.length(), t.length() ) << "Length mismatch.\n";
	EXPECT_LE( s.size(), s.capacity() ) << "Size exceeds capacity.\n";
	EXPECT_FALSE( s.empty() ) << "String is empty.\n";
	EXPECT_TRUE( s == t ) << "Strings are unequal.\n";
	EXPECT_STREQ( s.c_str(), t.c_str() ) << "C strings are unequal.\n";
}

TEST( klibstringConstructorFromString, WithReserve ) {
	// Ensure that the constructed string contains str. A specific capacity
	// was requested, so the actual capacity should equal that.
	char const *p = "Testing string.";
	klib::string t( p ); 
	klib::string s( t, 40 ); 
	EXPECT_EQ( s.size(), t.size() ) << "Size mismatch.\n";
	EXPECT_EQ( s.length(), t.length() ) << "Length mismatch.\n";
	EXPECT_EQ( s.capacity(), 40 ) <<
	    "Capacity is not what was requested.\n";
	EXPECT_FALSE( s.empty() ) << "String is empty.\n";
	EXPECT_TRUE( s == t ) << "Strings are unequal.\n";
	EXPECT_STREQ( s.c_str(), t.c_str() ) << "C strings are unequal.\n";
}

TEST( klibstringConstructorFromString, WithInsufficientReserve ) {
	// Ensure that the constructed string contains str. The requested
	// capacity is insufficient, so it must allocate the minimal amount.
	char const *p = "Testing.";
	klib::string t( p );
	klib::string s( t, 5 );
	EXPECT_EQ( s.size(), t.size() ) << "Size mismatch.\n";
	EXPECT_EQ( s.length(), t.length() ) << "Length mismatch.\n";
	EXPECT_EQ( s.capacity(), 9 ) << "Capacity is not minimal.\n";
	EXPECT_FALSE( s.empty() ) << "String is empty.\n";
	EXPECT_TRUE( s == t ) << "Strings are unequal.\n";
	EXPECT_STREQ( s.c_str(), t.c_str() ) << "C strings are unequal.\n";
}

TEST( klibstringConstructorFromString, FromUnallocated ) {
	// Ensure that the constructed string contains str. The requested
	// capacity is insufficient, so it must allocate the minimal amount.
	char const *p = "";
	klib::string t( p );
	klib::string s( t );
	EXPECT_EQ( s.size(), t.size() ) << "Size mismatch.\n";
	EXPECT_EQ( s.length(), t.length() ) << "Length mismatch.\n";
	EXPECT_LE( s.size(), s.capacity() ) << "Size exceeds capacity.\n";
	EXPECT_TRUE( s.empty() ) << "String is not empty.\n";
	EXPECT_TRUE( s == t ) << "Strings are unequal.\n";
	EXPECT_STREQ( s.c_str(), t.c_str() ) << "C strings are unequal.\n";
}
// |- Done: string( string const &str, size_t res = 0 ); ---------------------|

// |- Test: string( char const *cstrPtr, size_t num = 0 ) --------------------|
TEST( klibstringConstructorFromCString, NoReserve ) {
	// Ensure that the constructed string contains the original.
	char const* p = "Testing string.";
	klib::string s( p );
	ASSERT_EQ( s.size(), std::strlen( p ) + 1 ) <<
	    "Size mismatch.\n"; 
	EXPECT_EQ( s.length(), std::strlen( p ) ) <<
	    "Length mismatch.\n";
	EXPECT_LE( s.size(), s.capacity() ) <<
	    "Capacity is lower than size.\n";
	EXPECT_FALSE( s.empty() ) <<
	    "String is unexpectedly empty.\n";
	EXPECT_TRUE( s == p ) <<
	    "String does not consider itself equal with p.\n";
	EXPECT_STREQ( s.c_str(), p ) <<
	    "String does not compare as equal to p.\n";
}

TEST( klibstringConstructorFromCString, NoReserveDynamic ) {
	char const* p = "Testing string.";
	klib::string* s = new klib::string( p );
	ASSERT_EQ( s->size(), std::strlen( p ) + 1 ) <<
	    "Size mismatch.\n"; 
	EXPECT_EQ( s->length(), std::strlen( p ) ) <<
	    "Length mismatch.\n";
	EXPECT_LE( s->size(), s->capacity() ) <<
	    "Capacity is lower than size.\n";
	EXPECT_FALSE( s->empty() ) <<
	    "String is unexpectedly empty.\n";
	EXPECT_TRUE( *s == p ) <<
	    "String does not consider itself equal with p.\n";
	EXPECT_STREQ( s->c_str(), p ) <<
	    "String does not compare as equal to p.\n";
	EXPECT_EQ( (*s)[3], 't' ) <<
	    "Accessing fourth element fails.\n";
}

TEST( klibstringConstructorFromCString, NoReserveLong ) {
	char const* p = "Really somewhat longer than it needs to be \
but hey why not try it for the sake of making sure testing string.";
	klib::string s( p );
	ASSERT_EQ( s.size(), std::strlen( p ) + 1 ) <<
	    "Size mismatch.\n"; 
	EXPECT_EQ( s.length(), std::strlen( p ) ) <<
	    "Length mismatch.\n";
	EXPECT_LE( s.size(), s.capacity() ) <<
	    "Capacity is lower than size.\n";
	EXPECT_FALSE( s.empty() ) <<
	    "String is unexpectedly empty.\n";
	EXPECT_TRUE( s == p ) <<
	    "String does not consider itself equal with p.\n";
	EXPECT_STREQ( s.c_str(), p ) <<
	    "String does not compare as equal to p.\n";
}

TEST( klibstringConstructorFromCString, NoReserveEmpty ) {
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

TEST( klibstringConstructorFromCString, ExplicitNoReserve ) {
	char const* p = "Testing string.";
	klib::string s( p, 0 );
	ASSERT_EQ( s.size(), std::strlen( p ) + 1 ) <<
	    "Size mismatch.\n"; 
	EXPECT_EQ( s.length(), std::strlen( p ) ) <<
	    "Length mismatch.\n";
	EXPECT_LE( s.size(), s.capacity() ) <<
	    "Capacity is lower than size.\n";
	EXPECT_FALSE( s.empty() ) <<
	    "String is unexpectedly empty.\n";
	EXPECT_TRUE( s == p ) <<
	    "String does not consider itself equal with p.\n";
	EXPECT_STREQ( s.c_str(), p ) <<
	    "String does not compare as equal to p.\n";
}

TEST( klibstringConstructorFromCString, WithReserve ) {
	char const* p = "Testing string.";
	klib::string s( p, 321 );
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

TEST( klibstringConstructorFromCString, WithReserveLong ) {
	char const* p = "Really somewhat longer than it needs to be \
but hey why not try it for the sake of making sure testing string.";
	klib::string s( p, 1024 );
	ASSERT_EQ( s.size(), std::strlen( p ) + 1 ) <<
	    "Size mismatch.\n"; 
	EXPECT_EQ( s.length(), std::strlen( p ) ) <<
	    "Length mismatch.\n";
	EXPECT_EQ( s.capacity(), 1024 ) <<
	    "Capacity is not what was requested.\n";
	EXPECT_FALSE( s.empty() ) <<
	    "String is unexpectedly empty.\n";
	EXPECT_TRUE( s == p ) <<
	    "String does not consider itself equal with p.\n";
	EXPECT_STREQ( s.c_str(), p ) <<
	    "String does not compare as equal to p.\n";
}

TEST( klibstringConstructorFromCString, WithInsufficientReserve ) {
	char const* p = "Really somewhat longer than it needs to be \
but hey why not try it for the sake of making sure testing string.";
	klib::string s( p, 16 );
	ASSERT_EQ( s.size(), std::strlen( p ) + 1 ) <<
	    "Size mismatch.\n"; 
	EXPECT_EQ( s.length(), std::strlen( p ) ) <<
	    "Length mismatch.\n";
	EXPECT_LE( s.size(), s.capacity() ) <<
	    "Capacity is lower than size.\n";
	EXPECT_FALSE( s.empty() ) <<
	    "String is unexpectedly empty.\n";
	EXPECT_TRUE( s == p ) <<
	    "String does not consider itself equal with p.\n";
	EXPECT_STREQ( s.c_str(), p ) <<
	    "String does not compare as equal to p.\n";
}
// |- Done: string( char const *cstrPtr, size_t res = 0 ); --------------------|

// |- Test: string& operator=( string const &str ); ---------------------------|
TEST( klibstringAssignmentOperatorFromString, Normal ) {
	// Ensure that the string is copied correctly.
	char const *p = "Testing string.";
	klib::string t( p );
	klib::string s = t;
	EXPECT_EQ( s.size(), t.size() ) << "Size mismatch.\n";
	EXPECT_EQ( s.length(), t.length() ) << "Length mismatch.\n";
	EXPECT_LE( s.size(), s.capacity() ) << "Size exceeds capacity.\n";
	EXPECT_FALSE( s.empty() ) << "String is empty.\n";
	EXPECT_TRUE( s == t ) << "Strings do not compare as equal.\n";
	EXPECT_STREQ( s.c_str(), t.c_str() ) <<
	    "Strings do not compare as equal.";
}

TEST( klibstringAssignmentOperatorFromString, Empty ) {
	// Ensure that copying an empty string sets the string to empty.
	char const *p = "";
	klib::string t( p );
	klib::string s = t;
	EXPECT_EQ( s.size(), 1 ) << "Size mismatch.\n";
	EXPECT_EQ( s.length(), 0 ) << "Length mismatch.\n";
	// Capacity may be anything.
	EXPECT_TRUE( s.empty() ) << "String not empty.\n";
	EXPECT_TRUE( s == t ) << "Strings do not compare as equal.\n";
	EXPECT_STREQ( s.c_str(), t.c_str() ) <<
	    "Strings do not compare as equal.\n";
	EXPECT_EQ( s[0], '\0' ) << "Reading does not return a \\0.\n";
}

TEST( klibstringAssignmentOperatorFromString, LongToShort ) {
	// Ensure that copying a long string to a short one works.
	char const *p = "Other testing string that is probably long \
enough to have more memory allocated for it.";
	klib::string t( p );
	klib::string s( "Testing string." );
	s = t;
	EXPECT_EQ( s.size(), t.size() ) << "Size mismatch.\n";
	EXPECT_EQ( s.length(), t.length() ) << "Length mismatch.\n";
	EXPECT_LE( s.size(), s.capacity() ) << "Size exceeds capacity.\n";
	EXPECT_FALSE( s.empty() ) << "String is empty.\n";
	EXPECT_TRUE( s == t ) << "Strings do not compare as equal.\n";
	EXPECT_STREQ( s.c_str(), t.c_str() ) <<
	    "Strings do not compare as equal.\n";
}

TEST( klibstringAssignmentOperatorFromString, ShortToLong ) {
	// Ensure that copying a short string to a long one works.
	char const *p = "Testing string.";
	klib::string t( p );
	klib::string s( "Other testing string that is probably long \
enough to have more memory allocated for it." );
	s = t;
	EXPECT_EQ( s.size(), t.size() ) << "Size mismatch.\n";
	EXPECT_EQ( s.length(), t.length() ) << "Length mismatch.\n";
	EXPECT_LE( s.size(), s.capacity() ) << "Size exceeds capacity.\n";
	EXPECT_FALSE( s.empty() ) << "String is empty.\n";
	EXPECT_TRUE( s == t ) << "Strings do not compare as equal.\n";
	EXPECT_STREQ( s.c_str(), t.c_str() ) <<
	    "Strings do not compare as equal.\n";
}
// |- Done: string& operator=( string const &str ); ---------------------------|

// |- Test: string& operator=( char const *cstrPtr ); -------------------------|
TEST( klibstringAssignmentOperatorFromCString, Normal ) {
	// Ensure that the string stored is identical to the provided string,
	// and that the size and length are correct. Make sure whether it is
	// empty or not is shown correctly. 
	char const *p = "Testing string.";
	klib::string s;
	s = p;
	EXPECT_EQ( s.size(), std::strlen( p ) + 1 ) << "Size mismatch.\n";
	EXPECT_EQ( s.length(), std::strlen( p ) ) << "Length mismatch.\n";
	EXPECT_LE( s.size(), s.capacity() ) << "Size exceeds capacity.\n";
	EXPECT_FALSE( s.empty() ) << "String is empty.\n";
	EXPECT_TRUE( s == p ) << "Strings do not compare as equal.\n";
	EXPECT_STREQ( s.c_str(), p ) << "Strings do not compare as equal.\n";
}

TEST( klibstringAssignmentOperatorFromCString, Empty ) {
	// Ensure that assigning an empty string works.
	char const *p = "";
	klib::string s( "Test." );
	s = p;
	EXPECT_EQ( s.size(), 1 ) << "Size mismatch.\n";
	EXPECT_EQ( s.length(), 0 ) << "Length mismatch.\n";
	// Capacity may be anything.
	EXPECT_TRUE( s.empty() ) << "String not empty.\n";
	EXPECT_TRUE( s == p ) << "Strings do not compare as equal.\n";
	EXPECT_STREQ( s.c_str(), p ) << "Strings do not compare as equal.\n";
	EXPECT_EQ( s[0], '\0' ) <<
	    "Reading the first char does not return a \\0.\n";
}

TEST( klibstringAssignmentOperatorFromCString, LongToShort ) {
	// Ensure that coping a long string to a short one works.
	char const *p = "Other testing string that is probably long \
enough to have more memory allocated for it.";
	klib::string s( "Testing string." );
	s = p;
	EXPECT_EQ( s.size(), std::strlen( p ) + 1) << "Size mismatch.\n";
	EXPECT_EQ( s.length(), std::strlen( p ) ) << "Length mismatch.\n";
	EXPECT_LE( s.size(), s.capacity() ) << "Size exceeds capacity.\n";
	EXPECT_FALSE( s.empty() ) << "String is empty.\n";
	EXPECT_TRUE( s == p ) << "Strings do not compare as equal.\n";
	EXPECT_STREQ( s.c_str(), p ) << "Strings do not compare as equal.\n";
}

TEST( klibstringAssignmentOperatorFromCString, ShortToLong ) {
	// Ensure that copying a short string to a long one works.
	char const *p = "Testing string.";
	klib::string s( "Other testing string that is probably long \
enough to have more memory allocated for it." );
	s = p;
	EXPECT_EQ( s.size(), std::strlen( p ) + 1) << "Size mismatch.\n";
	EXPECT_EQ( s.length(), std::strlen( p ) ) << "Length mismatch.\n";
	EXPECT_LE( s.size(), s.capacity() ) << "Size exceeds capacity.\n";
	EXPECT_FALSE( s.empty() ) << "String is empty.\n";
	EXPECT_TRUE( s == p ) << "Strings do not compare as equal.\n";
	EXPECT_STREQ( s.c_str(), p ) << "Strings do not compare as equal.\n";
}
// |- Done: string& operator=( char const *cstrPtr ); ------------------------|

// |- Test: string& operator+=( const char* cstrPtr ); -----------------------|
TEST( klibstringAppendOperatorCString, Normal ) {
	// Ensure that appending a C string to a string that is not empty and
	// has plenty of free space.
	klib::string s( "Test" );
	s += "ing.";
	EXPECT_EQ( s.size(), 8 ) << "Size mismatch.\n";
	EXPECT_EQ( s.length(), 7 ) << "Length mismatch.\n";
	EXPECT_LE( s.size(), s.capacity() ) << "Size exceeds capacity.\n";
	EXPECT_FALSE( s.empty() ) << "String is empty.\n";
	EXPECT_TRUE( s == "Testing." ) <<
	    "Strings do not compare as equal.\n";
	EXPECT_STREQ( s.c_str(), "Testing." ) <<
	    "Strings do not compare as equal.\n";
}

TEST( klibstringAppendOperatorCString, NormalButFull ) {
	// Ensure that appending a C string to a string that is not empty and has
	// no more free space works.
	klib::string s( "Test", 1 );
	s += "ing?";
	EXPECT_EQ( s.size(), 8 ) << "Size mismatch.\n";
	EXPECT_EQ( s.length(), 7 ) << "Length mismatch.\n";
	EXPECT_LE( s.size(), s.capacity() ) << "Size exceeds capacity.\n";
	EXPECT_FALSE( s.empty() ) << "String is empty.\n";
	EXPECT_TRUE( s == "Testing?" ) <<
	    "Strings do not compare as equal.\n";
	EXPECT_STREQ( s.c_str(), "Testing?" ) <<
	    "Strings do not compare as equal.\n";
}

TEST( klibstringAppendOperatorCString, NormalAndOneOverfull ) {
	// Ensure that appending a C string when the resulting capacity is 32
	// greater than the original capacity works.
	klib::string s( "Testing string," );
	s += " which is specifically made to be 64 bytes long.";
	EXPECT_EQ( s.size(), 64 ) << "Size mismatch.\n";
	EXPECT_EQ( s.length(), 63 ) << "Length mismatch.\n";
	EXPECT_LE( s.size(), s.capacity() ) << "Size exceeds capacity.\n";
	EXPECT_FALSE( s.empty() ) << "String is empty.\n";
	EXPECT_TRUE( s ==
"Testing string, which is specifically made to be 64 bytes long." ) <<
	    "Strings do not compare as equal.\n";
	EXPECT_STREQ( s.c_str(),
"Testing string, which is specifically made to be 64 bytes long." ) <<
	    "Strings do not compare as equal.\n";
}

TEST( klibstringAppendOperatorCString, EmptyWithPlentyReserves ) {
	// Ensure that appending a C string to a string that is empty and has
	// more than enough reserved space works.
	klib::string s( 32 );
	s += "Testing string.";
	EXPECT_EQ( s.size(), 16 ) << "Size mismatch.\n";
	EXPECT_EQ( s.length(), 15 ) << "Length mismatch.\n";
	EXPECT_LE( s.size(), s.capacity() ) << "Size exceeds capacity.\n";
	EXPECT_FALSE( s.empty() ) << "String is empty.\n";
	EXPECT_TRUE( s == "Testing string." ) <<
	    "Strings do not compare as equal.\n";
	EXPECT_STREQ( s.c_str(), "Testing string." ) <<
	    "Strings do not compare as equal.\n";
}

TEST( klibstringAppendOperatorCString, EmptyWithBarelyEnoughReserves ) {
	// Ensure that appending a C string to a string that is empty and has
	// just enough reserved space works.
	klib::string s( 16 );
	s += "Testing string.";
	EXPECT_EQ( s.size(), 16 ) << "Size mismatch.\n";
	EXPECT_EQ( s.length(), 15 ) << "Length mismatch.\n";
	EXPECT_LE( s.size(), s.capacity() ) << "Size exceeds capacity.\n";
	EXPECT_FALSE( s.empty() ) << "String is empty.\n";
	EXPECT_TRUE( s == "Testing string." ) <<
	    "Strings do not compare as equal.\n";
	EXPECT_STREQ( s.c_str(), "Testing string." ) <<
	    "Strings do not compare as equal.\n";
}

TEST( klibstringAppendOperatorCString, EmptyWithInsufficientReserves ) {
	// Ensure that appending a C string to a string that is empty and has
	// some, but not enough, reserved space works.
	klib::string s( 8 );
	s += "Testing string.";
	EXPECT_EQ( s.size(), 16 ) << "Size mismatch.\n";
	EXPECT_EQ( s.length(), 15 ) << "Length mismatch.\n";
	EXPECT_LE( s.size(), s.capacity() ) << "Size exceeds capacity.\n";
	EXPECT_FALSE( s.empty() ) << "String is empty.\n";
	EXPECT_TRUE( s == "Testring string." ) <<
	    "Strings do not compare as equal.\n";
	EXPECT_STREQ( s.c_str(), "Testing string." ) <<
	    "Strings do not compare as equal.\n";
}

TEST( klibstringAppendOperatorCString, EmptyWithoutReserves ) {
	// Ensure that appending a C string to a string that is empty and has
	// no space reserved works.
	klib::string s;
	s += "Test?";
	EXPECT_EQ( s.size(), 6 ) << "Size mismatch.\n";
	EXPECT_EQ( s.length(), 5 ) << "Length mismatch.\n";
	EXPECT_LE( s.size(), s.capacity() ) << "Size exceeds capacity.\n";
	EXPECT_FALSE( s.empty() ) << "String is empty.\n";
	EXPECT_TRUE( s == "Test?" ) << "Strings do not compare as equal.\n";
	EXPECT_STREQ( s.c_str(), "Test?" ) <<
	    "Strings do not compare as equal.\n";
}
// |- Done: string& operator+=( const char* cstrPtr ); -----------------------|

// |- Test: string& operator+=( const char c ); ------------------------------|
TEST( klibstringAppendOperatorChar, Normal ) {
	// Ensure that appending a char to a string that is not empty and can
	// fit that char works.
	klib::string s( "Test" );
	s += '.';
	EXPECT_EQ( s.size(), 6 ) << "Size mismatch.\n";
	EXPECT_EQ( s.length(), 5 ) << "Length mismatch.\n";
	EXPECT_LE( s.size(), s.capacity() ) << "Size exceeds capacity.\n";
	EXPECT_FALSE( s.empty() ) << "String is empty.\n";
	EXPECT_TRUE( s == "Test." ) << "Strings do not compare as equal.\n";
	EXPECT_STREQ( s.c_str(), "Test." ) <<
	    "Strings do not compare as equal.\n";
}

TEST( klibstringAppendOperatorChar, NormalButFull ) {
	// Ensure that appending a char to a string that is not empty and has
	// no more free space works.
	klib::string s( "Test", 1 );
	s += '?';
	EXPECT_EQ( s.size(), 6 ) << "Size mismatch.\n";
	EXPECT_EQ( s.length(), 5 ) << "Length mismatch.\n";
	EXPECT_LE( s.size(), s.capacity() ) << "Size exceeds capacity.\n";
	EXPECT_FALSE( s.empty() ) << "String is empty.\n";
	EXPECT_TRUE( s == "Test?" ) << "Strings do not compare as equal.\n";
	EXPECT_STREQ( s.c_str(), "Test?" ) <<
	    "Strings do not compare as equal.\n";
}

TEST( klibstringAppendOperatorChar, EmptyWithReserves ) {
	// Ensure that appending a char to a string that is empty but has
	// reserved space works.
	klib::string s( 16 );
	s += '?';
	EXPECT_EQ( s.size(), 2 ) << "Size mismatch.\n";
	EXPECT_EQ( s.length(), 1 ) << "Length mismatch.\n";
	EXPECT_LE( s.size(), s.capacity() ) << "Size exceeds capacity.\n";
	EXPECT_FALSE( s.empty() ) << "String is empty.\n";
	EXPECT_TRUE( s == "?" ) << "Strings do not compare as equal.\n";
	EXPECT_STREQ( s.c_str(), "?" ) <<
	    "Strings do not compare as equal.\n";
}

TEST( klibstringAppendOperatorChar, EmptyWithoutReserves ) {
	// Ensure that appending a char to a string that is empty and has
	// no space reserved works.
	klib::string s;
	s += '?';
	EXPECT_EQ( s.size(), 2 ) << "Size mismatch.\n";
	EXPECT_EQ( s.length(), 1 ) << "Length mismatch.\n";
	EXPECT_LE( s.size(), s.capacity() ) << "Size exceeds capacity.\n";
	EXPECT_FALSE( s.empty() ) << "String is empty.\n";
	EXPECT_TRUE( s == "?" ) << "Strings do not compare as equal.\n";
	EXPECT_STREQ( s.c_str(), "?" ) <<
	    "Strings do not compare as equal.\n";
}
// |- Done: string& operator+=( const char c ); ------------------------------|

// |- Test: char const& operator[]( size_t pos ) const; ----------------------|
TEST( klibstringIndexOperatorRead, Normal ) {
	// Ensure that the characters read are the ones expected.
	char const* p = "Test.";
	klib::string s( p );
	EXPECT_EQ( s[0], p[0] ) << "Inequal at index 0.\n";
	EXPECT_EQ( s[3], p[3] ) << "Inequal at index 3.\n";
	EXPECT_EQ( s[5], p[5] ) << "Inequal at index 5.\n";
}

TEST( klibstringIndexOperatorRead, OutOfBounds ) {
	// Ensure that all out-of-bounds reads return a \0.
	klib::string s( "Test." );
	EXPECT_EQ( s[9], '\0' ) <<
	    "Out-of-bounds access did not return \\0.\n";
	EXPECT_EQ( s[42], '\0' ) <<
	    "Out-of-bounds access did not return \\0.\n";
	EXPECT_EQ( s[7-10], '\0' ) <<
	    "Out-of-bounds access did not return \\0.\n";
}

TEST( klibstringIndexOperatorRead, FakeString ) {
	// Ensure that all reads of a fake string return a \0.
	klib::string s;
	EXPECT_EQ( s[5], '\0' ) <<
	    "Access of an empty string did not return \\0.\n";
	EXPECT_EQ( s[7], '\0' ) <<
	    "Access of an empty string did not return \\0.\n";
}

TEST( klibstringIndexOperatorRead, FakeStringStayFake ) {
	// Ensure that a fake string is unaffected by read-only access to
	// a character other than 0.
	klib::string s;
	char a = s[9];
	(void)a;
	EXPECT_EQ( a, '\0' ) << "Read failed.\n";
	EXPECT_EQ( s.capacity(), 0 ) <<
	    "String was allocated due to mere reads.\n";
}
// |- Done: char const& operator[]( size_t pos ) const; ----------------------|

// |- Test: char& operator[]( size_t pos ); ----------------------------------|
TEST( klibstringIndexOperatorWrite, Normal ) {
	// Ensure that the characters written are stored.
	// When used this way, this function does not modify the string as a
	// whole, and thus tests on length may be skipped. See LastChar for
	// cases where they do directly modify it.
	klib::string s( "TTest." );
	s[0] = 'Q';
	EXPECT_EQ( s.c_str()[0], 'Q' ) << "Inequal at index 0.\n";
	s[1] = 'u';
	EXPECT_EQ( s.c_str()[1], 'u' ) << "Inequal at index 1.\n";
	s[5] = '!';
	EXPECT_EQ( s.c_str()[5], '!' ) << "Inequal at index 5.\n";
}
// |- Done: char& operator[]( size_t pos ); ----------------------------------|

// |- Test: bool operator==( char const* cstrPtr ) const; --------------------|
TEST( klibstringEqualityOperatorString, Normal ) {
	// Ensure that a string with contents does compare as equal to itself,
	// but does not compare as equal to an empty string, or a different
	// string (no matter whether the length is the same or not).
	klib::string t( "Test." );
	klib::string s( t );
	EXPECT_TRUE( s == klib::string( "Test." ) );
	EXPECT_FALSE( s == klib::string( "" ) );
	EXPECT_FALSE( s == klib::string( "Cake." ) );
	EXPECT_FALSE( s == klib::string( "Other test." ) );
}

TEST( klibstringEqualityOperatorString, Unallocated ) {
	// Ensure that an unallocated string compares as equal to an empty
	// string, and as unequal to a non-empty string.
	klib::string t;
	klib::string s( t );
	EXPECT_TRUE( s == klib::string( "" ) );
	EXPECT_FALSE( s == klib::string( "Test." ) );
}
// |- Done: bool operator==( char const* cstrPtr ) const; --------------------|

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
	klib::string s( p, 31 );
	EXPECT_EQ( s.capacity(), 31 ) <<
	    "Capacity set incorrectly.\n";
	EXPECT_GE( s.capacity(), s.size() ) <<
	    "Size exceeds capacity.\n";
}
// |- Done: size_t capacity() const; ------------------------------------------|

GTEST_API_ int main( int argc, char **argv ) {
	testing::InitGoogleTest( &argc, argv );
	return RUN_ALL_TESTS();
}

