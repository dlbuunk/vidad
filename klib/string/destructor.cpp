#include "string.h"

namespace klib {

string::~string() {
	delete[] strPtr_;
	// We don't delete tmpstr_ because it shouldn't point to anything
	// valid (it is probably pointing to where str_ just was)
}

} // namespace klib
