#ifndef STRING_H
#define STRING_H

namespace klib {

// Typical string class. Should probably inherit from basic_string, but I'll
// implement that later if I feel it is necessary.
class string {
    public:
		// Constructors::
	// Constructs the string to be empty.
	string(); //---
	// Copies an existing string.
	string( string const& str ); //---
	// Constructs the string to contain the C string passed to it.
	string( char const* str ); //---
    	
		// Operators:
	// Set the string contents from another string or C string.
	string& operator=( string const& str ); //---
	string& operator=( char const* str ); //---
	// Append another string, a C string, a single character or an integer
	// (hexadecimal).
	string& operator+=( string const& str ); //---
	string& operator+=( char const* str ); //---
	string& operator+=( const char c ); //---
	string& operator+=( const unsigned int num ); //---
	// Create a new string from this string and another string, a C string,
	// a character or an integer (hexadecimal).
	string operator+( string const& str ) const; //---
	string operator+( char const* str ) const; //---
	string operator+( const char c ) const; //---
	string operator+( const unsigned int num ) const; //---
	// Returns a (const) reference to the character at position pos.
	const char& operator[]( size_t pos ) const; //---
	char& operator[]( size_t pos ); //---
	// Compares the string with another string or a C string for
	// (in)equality.
	bool operator==( string const& str ) const; //---
	bool operator==( char const* str ) const; //---
	bool operator!=( string const& str ) const; //---
	bool operator!=( char const* str ) const; //---

		// Non-const functions:
	// Make sure that the allocated memory is either equal to size or
	// equal to the length of the stored string (whichever is greater).
	void reserve( size_t size = 0 ); //---
	// Sets the string to contain a single null byte.
	void clear(); //---
	// Clears the string and unallocates the memory, equivalent to clear()
	// followed by reserve().
	void free(); //---
	
		// Const functions:
	// Returns a pointer to the beginning of the string. This pointer will
	// stay valid at least until a non-const member function is called.
	const char* c_str() const; //---
	// Returns true if the string is empty, false otherwise.
	bool empty() const; //---
	// Returns the length of the string, without the \0 on the end.
	size_t length() const; //---
	// Returns the size of the string, with the \0 on the end.
	size_t size() const; //---
	// Returns the size of the allocated storage.
	size_t capacity() const; //---
		// Static functions (until we get a proper C library
		// implementation, they'll be here, they do exactly the same
		// thing the C ones do).
	static size_t strlen( char const* str );
	static char* strcpy( char* dest, char const* src );
	static char* strncpy( char* dest, char const* src, size_t num );
	static char* strcmp( char* dest, char const* src );
    private:
	// Size of the currently stored string.
	size_t strSize_;
	// Size of the allocated string.
	size_t allocSize_;
	// Pointer to the beginning of the string.
	char* str_;
	// Pointer to a temporary string (when memory is reallocated, the new
	// area is assigned here, then the values are copied, the old area
	// freed and finally this area is assigned to str_
	char* tmpstr_;
};

}

#endif // STRING_H
