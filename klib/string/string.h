//==---==  klib/string/string.h  ==----------------------==  *- C++ -*  ==---==>
//
// This header contains the string class definition and the inline
// functions that it provides. 
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
#ifndef STRING_H
#define STRING_H

#include <cstddef>

namespace klib {

// Explanation of the notation used for denoting function status:
// --- : function has no definition at all, and thus may not be used in tests
//       because it simply won't link.
// --  : function has a definition, but no tests have yet been written to
//       ensure that it works as expected. It may be used in tests, but may not
//       yet be written.
// -   : function has tests for it, but does not pass all the tests, or the
//       writer/maintainer feels like it is not yet ready. NOTE: If the tests
//       fail due to the absence/bugginess of a different function, this may be
//       omitted.
// +   : function is ready to be used in other code.
// ++  : function is complete, and unlikely to change in the coming time.
// ??? : function is dubious, maintainer should look at it.
//! Container for 8-bit text strings.
//
//! This container is intended to be used for working with text strings
//! in the kernel. Unlike most implementations, this is not simply a typedef
//! for basic_string<char> (not surprisingly, seeing as klib does not yet have
//! a basic_string class).
class string {
    public:
		// Constructors:
	//! \brief Constructs an empty string, with res bytes reserved.
	//!
	//! If res is zero, the string is not yet allocated.
	//
	//! \param res is the number of bytes that should be reserved.
	string( size_t res = 0 ); //++
	//! \brief Constructs a string with the same contents as str,
	//!        allocating at least res bytes of memory.
	//!
	//! The amount of memory allocated will always be enough to contain
	//! the string, including the \\0 at the end. If res is zero (default),
	//! an amount somewhat greater than this will be allocated in case the
	//! string must later be appended to. A non-zero res indicates that the
	//! user does care about the amount of allocated memory, and the string
	//! allocated will be as close to this amount as possible, usually
	//! exactly equal to the request, or equal to the size of the string,
	//! whichever is greater.
	//
	//! \param str is the klib::string the contents of which should be
	//!        copied.
	//! \param res is the number of bytes that should be reserved.
	string( string const& str, size_t res = 0 ); //++
	//! \brief Constructs a string with the C string pointed to by cstrPtr.
	//!
	//! The amount of memory allocated will always be enough to contain
	//! the string, including the \\0 at the end. If res is zero (default),
	//! an amount somewhat greater than this will be allocated in case the
	//! string must later be appended to. A non-zero res indicates that the
	//! user does care about the amount of allocated memory, and the string
	//! allocated will be as close to this amount as possible, usually
	//! exactly equal to the request, or equal to the size of the string,
	//! whichever is greater.
	//!
	//! Note: Here, the C string's size is defined as the output of
	//! strlen( cstrPtr ) + 1.
	//
	//! \param *cstrPtr is a pointer to the C string that should be copied.
	//! \param res is the number of bytes that should be reserved.
	string( char const* cstrPtr, size_t res = 0 ); //++
    	
		// Destructor:
	~string(); //++

		// Operators:
	//! \brief Overwrite the contents of this string with that of str.
	//!
	//! The resulting string will compare as equal to str, but modifying
	//! the contents of one will not modify the contents of the other, and
	//! will continue to exist after the other goes out of scope.
	//!
	//! There is no guarantee that the amount of allocated space will be
	//! equal, and no checks are made for early \\0 characters in str.
	//
	//! \param str is the string the contents of which should be copied.
	string& operator=( string const& str ); //++
	//! \brief Overwrite the contents of this string with a copy of the C
	//!        string cstrPtr points to.
	//!
	//! The resulting string will compare as equal to cstrPtr, and using
	//! klib::strcmp() on the two will yield 1, but modifying the contents
	//! of this string will not change the C string cstrPtr is pointing to.
	//
	//! \param *cstrPtr is a pointer to the C string that should be copied.
	string& operator=( char const* cstrPtr ); //++
	//! \brief Appends the contents of string str to this string.
	//!
	//! This is equivalent to *this = *this + str, but avoids creating a
	//! temporary object, and should thus be used instead when possible.
	//
	//! \param str is the string the contents of which should be added.
	string& operator+=( string const& str ); //++
	//! \brief Appends the C string pointed to by cstrPtr to this string.
	//!
	//! This is equivalent to *this = *this + cstrPtr, but avoids creating a
	//! temporary object, and should thus be used instead when possible.
	//
	//! \param cstrPtr is a pointer to the C string that should be added.
	string& operator+=( char const* cstrPtr ); //++
	//! \brief Appends the character c to this string.
	//!
	//! This is equivalent to *this = *this + c, but avoids creating a
	//! temporary object, and should thus be used instead when possible.
	//! Although using this operator is somewhat faster than += "c", it
	//! is still far slower than appending an entire string (primarily due
	//! to the amount of allocated storage being checked, and the null
	//! terminating character being written).
	//
	//! \param c is the character to be appended.
	string& operator+=( const char c ); //++
	//! \brief Returns a temporary string with the concatenation of this
	//!        string and str.
	//! 
	//! This is implemented via the += operator, and is thus guaranteed to
	//! be slower.
	//
	//! \param str is the string this string should be concatenated with.
	string operator+( string const& str ) const; //++
	//! \brief Returns a temporary string with the concatenation of this
	//!        string and the C string pointed to by cstrPtr.
	//! 
	//! This is implemented via the += operator, and is thus guaranteed to
	//! be slower.
	//
	//! \param cstrPtr is a pointer to the string this string should be
	//!        concatenated with.
	string operator+( char const* cstrPtr ) const; //++
	//! \brief Returns a temporary copy of this string, with character c
	//!        appended to it.
	//! 
	//! This is implemented via the += operator, and is thus guaranteed to
	//! be slower.
	//
	//! \param c is a character that should be appended to the temporary
	//!        string.
	string operator+( const char c ) const; //++
	//! \brief Returns a const reference to the character at position pos,
	//!
	//! If that position is out of bounds, a reference to a character
	//! containing a \\0 is returned. It is not advised to store a reference
	//! or pointer to this value. If you want/need pointer-like access to
	//! the string, please submit an issue for iterators to be made.
	//
	//! \param pos is the position of the character to be returned, with
	//!        the first character at 0.
	const char& operator[]( size_t pos ) const; //++
	//! \brief Returns a reference to the character at position pos,
	//!
	//! If that position is out of bounds, a reference to a character
	//! containing a \\0 is returned. Modifying this character will not
	//! have any effect on the string. It is not advised to store a 
	//! reference or pointer to this value, and it is certainly not advised
	//! to ever write to this reference or pointer if you do decide to make
	//! one. If you want/need pointer-like access to the string, please
	//! submit an issue for iterators to be made.
	//
	//! \param pos is the position of the character to be returned, with
	//!        the first character at 0.
	char& operator[]( size_t pos ); //++
	//! \brief Returns true if this string and str have the same contents,
	//!        and false otherwise.
	//!
	//! In order to speed up this operation, the size of the strings is
	//! checked before a byte-by-byte comparison is done. Due to this,
	//! if string one has more \\0 characters at the end, the two will still
	//! compare as different. On the other hand, you shouldn't have multiple
	//! \\0 characters in a string, so running validate() on both prior to
	//! comparing should deal with this if you have reason for concern.
	//
	//! \param str is the string that is to be compared with.
	bool operator==( string const& str ) const; //++
	//! \brief Returns true if this string is the same as the C string
	//!        pointed to be cstrPtr, and false otherwise.
	//!
	//! Before doing a byte-by-byte comparison, the length of the C string
	//! is checked. If it is different from the length of this string, the
	//! strings are considered different, even if strcmp would normally
	//! return 0 (for example, if this string had an early \\0 character).
	//! However, strings should not contain early \\0 characters, and so
	//! this can be avoided by running validate() on this string if there
	//! is reason to believe this may occur.
	//
	//! \param cstrPtr is a pointer to the C string that is to be compared
	//!        with.
	bool operator==( char const* cstrPtr ) const; //++
	//! \brief Returns true if this string and str do not have the same
	//!        contents, and true otherwise.
	//!
	//! In order to speed up this operation, the size of the strings is
	//! checked before a byte-by-byte comparison is done. Due to this,
	//! if string one has more \\0 characters at the end, the two will still
	//! compare as different. On the other hand, you shouldn't have multiple
	//! \\0 characters in a string, so running validate() on both prior to
	//! comparing should deal with this if you have reason for concern.
	//
	//! \param str is the string that is to be compared with.
	bool operator!=( string const& str ) const; //++
	//! \brief Returns true if this string is different from the C string
	//!        pointed to be cstrPtr, and false otherwise.
	//!
	//! Before doing a byte-by-byte comparison, the length of the C string
	//! is checked. If it is different from the length of this string, the
	//! strings are considered different, even if strcmp would normally
	//! return 0 (for example, if this string had an early \\0 character).
	//! However, strings should not contain early \\0 characters, and so
	//! this can be avoided by running validate() on this string if there
	//! is reason to believe this may occur.
	//
	//! \param cstrPtr is a pointer to the C string that is to be compared
	//!        with.
	bool operator!=( char const* cstrPtr ) const; //++

		// Non-const functions:
	// Make sure that the allocated memory is either equal to size or
	// equal to the length of the stored string (whichever is greater).
	//! \brief Attempt to make the amount of allocated memory equal to size
	//!
	//! 
	void reserve( size_t size = 0 ); //++
	// Sets the string to contain a single null byte. Equivalent to
	// truncateAt( 0 ), but does not return anything.
	string& clear(); //++
	// Clears the string and unallocates the memory, equivalent to
	// truncateAt( 0 ) followed by reserve().
	string& drop(); //++
	// Checks whether the string contains any \0 characters, and truncates
	// at them if it does.
	string& validate(); //++
	// Truncates the string at position pos (i.e. keeping pos characters).
	string& truncateAt( size_t pos ); //++
	// Appends a string to the string.
	string& append( string const& str ); //++
	// Appends a C string to the string.
	string& append( char const* cstrPtr ); //++
	// Appends a character to the string.
	string& append( char c ); //++
	// Appends an int in decimal to the string. Digits is the minimal number
	// of digits to append.
	string& appendDecimal( int d, size_t digits = 0 ); //+
	// Appends an unsigned int in hexadecimal (lowercase) to the string.
	// Digits is the minimal number of digits to append.
	string& appendHex( unsigned int val, size_t digits = 0 ); //+
	// Appends an unsigned int in binary to the string. Digits is the
	// minimal number of digits to append.
	string& appendBinary( unsigned int b, size_t digits = 0 ); //+
	// Appends an unsigned int in octal to the string. Digits is the minimal
	// number of digits to append.
	string& appendOctal( unsigned int o, size_t digits = 0 ); //+
	// Inserts string str into this string, starting at positino pos.
	void insert( string const& str, size_t pos ); //+
	// Inserts C string cstrPTr into this string, starting at position pos.
	void insert( char const* cstrPtr, size_t pos ); //+
	// Inserts char c at position pos.
	void insert( char c, size_t pos ); //+
	
		// Const functions:
	// Returns a pointer to the beginning of the string. This pointer will
	// stay valid at least until a non-const member function is called.
	// NOTE: Although this function is not tested by itself, it has been
	// tested together with other functions, and should /usually/ work.
	const char* c_str() const; //+
	// Returns true if the string is empty, false otherwise.
	bool empty() const; //++
	// Returns the size of the string, with the \0 on the end.
	size_t size() const; //++
	// Returns the length of the string, without the \0 on the end.
	size_t length() const; //++
	// Returns the size of the allocated storage. Please do not depend on
	// this being a certain size unless you call reserve() for it.
	size_t capacity() const; //++
	// If len is non-zero, returns a string of length len, starting from
	// position pos. 
	// If len is zero, returns a string starting from position pos, and
	// going on until the end of the string.
	// If pos is greater than the length of the string, this returns an
	// empty string. If len requires for characters past the end of the
	// string to be given, the request will be ignored and only as many as
	// are available will be given.
	// NOTE: May have strange results if string contains \0 chars, so run
	// validate() first if you're not sure.
	string subStr( size_t pos, size_t len = 0 ) const; //+

	//! This is an internally used variable that denotes to what number
	//! the size of the allocated memory should be rounded to. As a rule
	//! of thumb, one can assume that this is the maximum free space that
	//! will be left in a newly-allocated string, but it's generally a
	//! better idea to check explicitly with capacity() if the amount
	//! reserved is important.
	static const size_t roundto_ = 32;
    private:
	// Size of the currently stored string.
	size_t strSize_;
	// Size of the allocated string.
	mutable size_t allocSize_;
	// Pointer to the beginning of the string.
	mutable char* strPtr_;
	// This is a character that is returned when an out-of-bounds access
	// is performed. May be removed later on.
	mutable char nullval_;
};

} // namespace klib

#include <klib/string_inline.h>


#endif // STRING_H

