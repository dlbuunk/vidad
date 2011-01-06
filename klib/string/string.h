//==---==  klib/string/string.h  ==--------------------==  *- C++ -*  ==---==>
//
// This header contains the string class definition and the inline
// functions that it provides. 
//
//==-----------------------------------------------------------------------==>
#ifndef STRING_H
#define STRING_H

typedef unsigned int size_t;

namespace klib {

// Typical string class. Should probably inherit from basic_string, but I'll
// implement that later if I feel it is necessary.
//
// Explanation of the notation used for denoting function status:
// --- : function has no definition at all, and thus may not be used in tests
//       because it simply won't link.
// --  : function has a definition, but no tests have yet been written to
//       ensure that it works as expected. It may be used in tests, but may not
//       yet be written.
// -   : function has tests for it, but does not pass all the tests, or the
//       writer/maintainer feels like it is not yet ready.
// +   : function is ready to be used in other code.
// ++  : function is fairly complete, and unlikely to change in the coming time.
// ??? : function is dubious, maintainer should look at it.
class string {
    public:
		// Constructors::
	// Constructs an empty string, with res bytes reserved. If res is zero,
	// the string is not yet allocated.
	string( size_t res = 0 ); //--
	// Simple explanation: Will copy the string. If res is non-zero, it will
	// try to limit itself to res bytes, but will allocate more (although
	// still as few as possible) if necessary.
	// Full explanation:
	// Copies an existing string. If res is non-zero, res bytes or the
	// number of bytes equal to the size of the string are allocated,
	// whichever is greater. If res is zero, the constructor may decide how
	// many bytes it wishes to allocate itself.
	string( string const& str, size_t res = 0 ); //--
	// Simple explanation: copies the C string pointed to by cstrPtr. If
	// num is non-zero, copies that many characters then null-terminates if
	// necessary. If res is non-zero, the constructor tries to allocate
	// res bytes, but will allocate more if it needs to (however, it will
	// still attempt to allocate as little as possible).
	// Full explanation:
	// If num is zero, this constructs a string that contains a copy of the
	// C string pointed to by cstrPtr (thus, all and including the first \0
	// it comes across. If res is non-zero, either res bytes or the size of
	// the number of bytes equal to the size of the string will be
	// allocated, whichever is greater.
	// If num is non-zero, this constructs a string that contains a copy of
	// the first num characters of the pointed-to string. If the last
	// character is not a \0, it will append a \0 to that. If res is
	// non-zero and the last byte is not a \0, either res or num+1 bytes
	// will be allocated, whichever is greater. If res is non-zero and the
	// last byte is a \0, either res or num bytes will be allocated.
	// NOTE: If you want to unallocate the string, use drop() instead.
	string( char const* cstrPtr, size_t num = 0, size_t res = 0 ); //--
    	
		// Destructor:
	~string(); //--

		// Operators:
	// Set the string contents from another string or C string.
	string& operator=( string const& str ); //--
	string& operator=( char const* cstrPtr ); //--
	// Append another string, a C string, a single character or an integer
	// (hexadecimal).
	string& operator+=( string const& str ); //--
	string& operator+=( char const* cstrPtr ); //--
	string& operator+=( const char c ); //--
	string& operator+=( const unsigned int num ); //--
	// Create a new string from this string and another string, a C string,
	// a character or an integer (hexadecimal).
	string operator+( string const& str ) const; //--
	string operator+( char const* cstrPtr ) const; //--
	string operator+( const char c ) const; //--
	string operator+( const unsigned int num ) const; //--
	// Returns a (const) reference to a char.
	const char& operator[]( size_t pos ) const; //-
	char& operator[]( size_t pos ); //-
	// Compares the string with another string or a C string for
	// (in)equality. The entire string is compared, even if it contains
	// \0 bytes, so a string containing "A\0B\0" will not be equal to
	// the C string "A\0".
	bool operator==( string const& str ) const; //--
	bool operator==( char const* cstrPtr ) const; //--
	bool operator!=( string const& str ) const; //--
	bool operator!=( char const* cstrPtr ) const; //--

		// Non-const functions:
	// Make sure that the allocated memory is either equal to size or
	// equal to the length of the stored string (whichever is greater).
	void reserve( size_t size = 0 ); //--
	// Sets the string to contain a single null byte.
	void clear(); //--
	// Clears the string and unallocates the memory, equivalent to clear()
	// followed by reserve().
	void drop(); //--
	// Appends a string to the string.
	void appendString( string const& str ); //---
	// Appends a C string to the string.
	void appendCString( char const* cstrPtr ); //---
	// Appends a character to the string.
	void appendChar( char c ); //---
	// Appends an int in decimal to the string.
	void appendDecimal( int d ); //---
	// Appends an unsigned int in hexadecimal (lowercase) to the string.
	void appendHex( unsigned int h ); //---
	// Appends an unsigned int in binary to the string.
	void appendBinary( unsigned int b ); //---
	// Appends an unsigned int in octal to the string.
	void appendOctal( unsigned int o ); //---
	
		// Const functions:
	// Returns a pointer to the beginning of the string. This pointer will
	// stay valid at least until a non-const member function is called.
	const char* c_str() const; //--
	// Returns true if the string is empty, false otherwise.
	bool empty() const; //-
	// Returns the size of the string, with the \0 on the end.
	size_t size() const; //-
	// Returns the length of the string, without the \0 on the end.
	size_t length() const; //-
	// Returns the size of the allocated storage. Please do not depend on
	// this being a certain size unless you call reserve() for it.
	size_t capacity() const; //-
	// If len is non-zero, returns a string of length len, starting from
	// position pos.
	// If len is zero, returns a string starting from position pos, and
	// going on until the next \0.
	string substr( size_t pos, size_t len = 0 ) const; //---

		// Static functions (until we get a proper C library
		// implementation, they'll be here, they do exactly the same
		// thing the C ones do).
	static size_t strlen( char const* str ); //---
	static char* strcpy( char* dest, char const* src ); //---
	static char* strncpy( char* dest, char const* src, size_t num ); //---
	static char* strcmp( char* dest, char const* src ); //---

    private:
	// Size of the currently stored string.
	size_t strSize_;
	// Size of the allocated string.
	size_t allocSize_;
	// Pointer to the beginning of the string.
	char* strPtr_;
	// This is a character that is returned when an out-of-bounds access
	// is performed. May be removed later on.
	mutable char nullval_;
};

#include "inline.h"

}

#endif // STRING_H

