//==---==  klib/string/destructor.cpp ==---------------==  *- C++ -*  ==---==>
//
// This source file contains the destructor for the klib::string class.
//
//==-----------------------------------------------------------------------==>
#include "string.h"

namespace klib {

string::~string() {
	delete[] strPtr_;
}

} // namespace klib

