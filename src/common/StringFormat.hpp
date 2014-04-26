// ------------------------------------------------------------------
// StringFormat - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_STRINGFORMAT_H
#define SD_STRINGFORMAT_H

#include "ConceptTraits.hpp"

#include <string>
#include <vector>
#include <type_traits>

namespace stardazed {
namespace str {


// defer numerical types to std
template<typename T>
std::string toString(const T t, typename std::enable_if_t<std::is_arithmetic<T>::value>* = nullptr) {
	return std::to_string(t);
}


// basic conversions
std::string toString(const bool b);
std::string toString(const char * cs);
std::string toString(std::string s);


// very basic vector<T> serialization
// change this to more of a Sequence concept template later
template <typename T>
std::string toString(const std::vector<T>& vt) {
	std::string s {"["};
	for (T& t : vt)
		s += toString(t) + ",";
	return s + "]";
}


} // ns str
} // ns stardazed

#endif
