// ------------------------------------------------------------------
// StringFormat.cpp - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "StringFormat.hpp"

namespace stardazed {
namespace str {


namespace {
	const std::string TRUE_STRING { "true" }, FALSE_STRING { "false" };
}

std::string toString(const bool b) {
	return b ? TRUE_STRING : FALSE_STRING;
}

std::string toString(const char* cs) {
	return cs;
}

std::string toString(std::string s) {
	return s;
}



} // ns str
} // ns stardazed
