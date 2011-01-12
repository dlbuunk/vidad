//==---==  klib/string/string.hpp  ==--------------------==  *- C++ -*  ==---==>
//
//! \file klib/string/string.hpp
//! \brief Contains the string class definition and the inline functions that it
//! provides. 
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
#ifndef STRING_HPP
#define STRING_HPP

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

//! \brief Container for 8-bit-wide text strings.
//!
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
	//!
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
	//!
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
	//!
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
	//!
	//! \param str is the string the contents of which should be copied.
	string& operator=( string const& str ); //++
	//! \brief Overwrite the contents of this string with a copy of the C
	//!        string cstrPtr points to.
	//!
	//! The resulting string will compare as equal to cstrPtr, and using
	//! klib::strcmp() on the two will yield 1, but modifying the contents
	//! of this string will not change the C string cstrPtr is pointing to.
	//!
	//! \param *cstrPtr is a pointer to the C string that should be copied.
	string& operator=( char const* cstrPtr ); //++
	//! \brief Appends the contents of string str to this string.
	//!
	//! This is equivalent to *this = *this + str, but avoids creating a
	//! temporary object, and should thus be used instead when possible.
	//!
	//! \param str is the string the contents of which should be added.
	string& operator+=( string const& str ); //++
	//! \brief Appends the C string pointed to by cstrPtr to this string.
	//!
	//! This is equivalent to *this = *this + cstrPtr, but avoids creating a
	//! temporary object, and should thus be used instead when possible.
	//!
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
	//!
	//! \note The speed of this function is likely to change in the coming
	//!       rewrite.
	//!
	//! \param c is the character to be appended.
	string& operator+=( const char c ); //++
	//! \brief Returns a temporary string with the concatenation of this
	//!        string and str.
	//! 
	//! This is implemented via the += operator, and is thus guaranteed to
	//! be slower.
	//!
	//! \param str is the string this string should be concatenated with.
	string operator+( string const& str ) const; //++
	//! \brief Returns a temporary string with the concatenation of this
	//!        string and the C string pointed to by cstrPtr.
	//! 
	//! This is implemented via the += operator, and is thus guaranteed to
	//! be slower.
	//!
	//! \param cstrPtr is a pointer to the string this string should be
	//!        concatenated with.
	string operator+( char const* cstrPtr ) const; //++
	//! \brief Returns a temporary copy of this string, with character c
	//!        appended to it.
	//! 
	//! This is implemented via the += operator, and is thus guaranteed to
	//! be slower.
	//!
	//! \param c is a character that should be appended to the temporary
	//!        string.
	string operator+( const char c ) const; //++
	//! \brief Returns a const reference to the character at position pos,
	//!
	//! If that position is out of bounds, a reference to a character
	//! containing a \\0 is returned. It is not advised to store a reference
	//! or pointer to this value. If you want/need pointer-like access to
	//! the string, please submit an issue for iterators to be made.
	//!
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
	//!
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
	//!
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
	//!
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
	//!
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
	//!
	//! \param cstrPtr is a pointer to the C string that is to be compared
	//!        with.
	bool operator!=( char const* cstrPtr ) const; //++

		// Non-const functions:
	//! \brief Attempt to make the amount of allocated memory equal to size.
	//!
	//! First of all, it is checked that the requested size is sufficient
	//! to fit the string. If it is not, size is increased to be the size
	//! of the string. After that, if the amount of allocated memory in
	//! bytes is not equal to size, the amount is reallocated as
	//! necessary. Due to the way memory is currently allocated, this will
	//! always cause the string to be copied. It is advised to only use
	//! this function if you need to decrease the amount of memory, or if
	//! you are going to do a lot of appends one after another. For all
	//! other things, the string should do a fairly decent job of guessing
	//! how much memmory it needs.
	//!
	//! \param size is the amount of memory that the user wishes to be
	//!        allocated.
	void reserve( size_t size = 0 ); //++
	//! \brief Set the string to contain an empty string.
	//!
	//! This does not free the memory, use drop() for that. The function
	//! can be chained ( str.clear().append("Text") ), but using clear()
	//! as any function except the first in the chain somewhere in the
	//! vidad code will likely frustrate the other developers.
	string& clear(); //++
	//! \brief Set the string to contain an empty string and free the
	//!        memory reserved.
	//!
	//! This does exactly the same thing as clear(), but somewhat more
	//! effectively and also gets rid of the memory. Useful if the string
	//! is not due to go out of scope for a while, but you're sure you
	//! won't ever need it.
	string& drop(); //++
	//! \brief Ensures that the string does not contain errors.
	//!
	//! The exact behaviour of this function is not currently defined. 
	//! \deprecated This function currently doesn't do anything, seeing as
	//!             there is no known way to cause the string to contain
	//!             errors.
	string& validate(); //++
	//! \brief Ensures that the string is no more than n characters long.
	//!
	//! Thanks to the first element having index 0, terminating the string
	//! at character n and leaving n characters is the same thing. Thus,
	//! this has the same effect as setting character n of a C string to
	//! \\0.
	//!
	//! \param n is the number of characters left in the string.
	string& truncateAt( size_t n ); //++
	//! \brief Appends the contents of string str to this string.
	//!
	//! This is equivalent to *this = *this + str, but avoids creating a
	//! temporary object, and should thus be used instead when possible.
	//!
	//! \param str is the string the contents of which should be added.
	string& append( string const& str ); //++
	//! \brief Appends the C string pointed to by cstrPtr to this string.
	//!
	//! This is equivalent to *this = *this + cstrPtr, but avoids creating a
	//! temporary object, and should thus be used instead when possible.
	//!
	//! \param cstrPtr is a pointer to the C string that should be added.
	string& append( char const* cstrPtr ); //++
	//! \brief Appends the character c to this string.
	//!
	//! This is equivalent to *this = *this + c, but avoids creating a
	//! temporary object, and should thus be used instead when possible.
	//! Although using this function is somewhat faster than .append("c"),
	//! it is still far slower than appending an entire string (primarily
	//! due to the amount of allocated storage being checked, and the null
	//! terminating character being written).
	//!
	//! \note The speed of this function is likely to change in the coming
	//!       rewrite.
	//!
	//! \param c is the character to be appended.
	string& appendChar( char c ); //++
	//! \brief Append a signed decimal number to this string.
	//!
	//! Requesting more digits than a signed int can convert to (10) will
	//! cause the number you request to be lowered.
	//!
	//! \param d is the number to be appended.
	//! \param digits is the minimum number of digits the number should
	//!        have. 
	string& appendDecimal( int d, size_t digits = 0 ); //+
	//! \brief Append an unsigned, uppercase hexadecimal number to this
	//!        string. 
	//!
	//! Requesting more digits than an usigned int can convert to (8) will
	//! cause the number you request to be lowered.
	//!
	//! \note There will be no 0x preceding the number.
	//!
	//! \param h is the number to be appended.
	//! \param digits is the minimum number of digits the number should
	//!        have. 
	string& appendHex( unsigned int h, size_t digits = 0 ); //+
	//! \brief Append an unsigned binary number to this string. 
	//!
	//! Requesting more digits than an usigned int can convert to (32) will
	//! cause the number you request to be lowered.
	//!
	//! \param b is the number to be appended.
	//! \param digits is the minimum number of digits the number should
	//!        have. 
	string& appendBinary( unsigned int b, size_t digits = 0 ); //+
	//! \brief Append an unsigned octal number to this string. 
	//!
	//! Requesting more digits than an usigned int can convert to (11) will
	//! cause the number you request to be lowered.
	//!
	//! \note There will be no 0 preceding the number.
	//!
	//! \param o is the number to be appended.
	//! \param digits is the minimum number of digits the number should
	//!        have. 
	string& appendOctal( unsigned int o, size_t digits = 0 ); //+
	//! \brief Insert the contents of string str starting at position pos
	//!        of this string, moving the rest of the existing contents
	//!        upwards.
	//!
	//! \param str is the string to be inserted.
	//! \param pos is the position where the inserted string should begin.
	void insert( string const& str, size_t pos ); //+
	//! \brief Insert the C string pointed to by cstrPtr starting at
	//!        position pos of this string, moving the rest of the existing
	//!        contents upwards.
	//!
	//! \param cstrPtr is the string to be inserted.
	//! \param pos is the position where the inserted string should begin.
	void insert( char const* cstrPtr, size_t pos ); //+
	//! \brief Insert character c at position pos of this string, moving
	//!        the rest of the existing contents upwards.
	//!
	//! \param c is the character to be inserted.
	//! \param pos is the position where the inserted character should be
	//!        placed.
	void insert( char c, size_t pos ); //+
	
		// Const functions:
	//! \brief Returns a const pointer to a C string that is identical to
	//!        the contents of this string at the time of calling c_str().
	//!
	//! The pointer will remain valid at least until a non-const member
	//! function of this object is called.
	const char* c_str() const; //+
	//! \brief Returns true if this string is empty, false otherwise.
	bool empty() const; //++
	//! \brief Returns the length of the string.
	//!
	//! This is the number of characters in the string, not counting the
	//! (non-existant) terminating \\0. If the string does not contain
	//! \\0 characters, the return value will be equal to that of strlen
	//! (this->c_str()).
	size_t size() const; //++
	//! \brief Returns the length of the string.
	//!
	//! This is the number of characters in the string, not counting the
	//! (non-existant) terminating \\0. If the string does not contain
	//! \\0 characters, the return value will be equal to that of strlen
	//! (this->c_str()).
	size_t length() const; //++
	//! \brief Returns the amount of allocated storage space for this
	//!        string, in bytes.
	size_t capacity() const; //++
	//! \brief Returns a substring of this string.
	//!
	//! If len is zero, this returns a string starting at position pos
	//! and going on until the end of the string. If a len is specified,
	//! the string will be no longer than len, but may be shorter if the
	//! string is too short. An empty string will be returned if pos is
	//! out of bounds.
	//!
	//! \note At the moment, this function may reactly strangely to strings
	//!       that contain \\0 characters, but this should soon be
	//!       resolved.
	//! 
	//! \param pos is the position from which the new string should start.
	//! \param len is the desired length of the substring.
	string subStr( size_t pos, size_t len = 0 ) const; //+

	//! \brief Value used for guessing the amount of space that should be
	//!        allocated for this string.
	//!
	//! This is an internally used variable that denotes to what number
	//! the size of the allocated memory should be rounded to. As a rule
	//! of thumb, one can assume that this is the maximum free space that
	//! will be left in a newly-allocated string, but it's generally a
	//! better idea to check explicitly with capacity() if the amount
	//! reserved is important.
	static const size_t roundto_ = 32;
    private:
	//! \brief Allocates size bytes for the string, and moves the current
	//!        contents there.
	//!
	//! You should make sure that size is not less than the current size
	//! of the string, or bad things may happen.
	//! 
	//! \param size is the size of the new memory to be allocated.
	void changeAlloc( size_t size ) const;
	//! \brief Guesses how much should be allocated, based on input.
	size_t calcAllocSize( size_t size ) const;
	//! \brief Size of the currently stored string.
	size_t strSize_;
	//! \brief Amount of memory allocated for the main string.
	mutable size_t allocSize_;
	//! \brief Pointer to the beginning of the string.
	mutable char* strPtr_;
	//! \brief This is a character that is returned when an out-of-bounds
	//!        access is performed.
	mutable char nullval_;
};

} // namespace klib

#include <klib/string_inline.hpp>

#endif // STRING_HPP

