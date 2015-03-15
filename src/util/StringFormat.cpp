// ------------------------------------------------------------------
// StringFormat.cpp - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "StringFormat.hpp"

namespace stardazed {


namespace detail {
	void concatAsString(std::string&) {}
}


std::string toString(std::nullptr_t) {
	return std::string("nullptr", 7);
}

std::string toString(const bool b) {
	return b ? std::string("true", 4) : std::string("false", 5);
}

std::string toString(const char* cs) {
	return cs;
}

std::string toString(std::string s) {
	return s;
}


} // ns stardazed
