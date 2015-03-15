// ------------------------------------------------------------------
// StringFormat - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_STRINGFORMAT_H
#define SD_STRINGFORMAT_H

#include "system/Config.hpp"
#include <string>
#include <vector>
#include <type_traits>

namespace stardazed {


// defer numerical types to std
template<typename T>
std::string toString(const T t, typename std::enable_if_t<std::is_arithmetic<T>::value>* = nullptr) {
	return std::to_string(t);
}

// built-in arrays
template <typename T>
std::string toString(T* at, size_t N) {
	bool rest = false;
	std::string s {'[', 1};
	for (T *pt = at, *et = at + N; pt != et; ++pt) {
		if (rest)
			s += ',';
		else
			rest = true;
		
		s += toString(*pt);
	}
	return s + ']';
}

template <typename C>
// requires Container<C>
std::string toString(const C& ct, typename C::value_type* = nullptr) {
	using T = typename C::value_type;
	
	bool rest = false;
	std::string s {'[', 1};
	for (const T& t : ct) {
		if (rest)
			s += ',';
		else
			rest = true;
		s += toString(t);
	}
	return s + ']';
}


// pointers
std::string toString(std::nullptr_t);

template <typename T>
std::string toString(const T* p) {
	return p ? std::string("(Pointer)", 9) : std::string("nullptr", 7);
}


// basic conversions
std::string toString(const bool b);
std::string toString(const char * cs);
std::string toString(std::string s);


// concatAsString(...)
// returns a string with the toString() representations of each
// passed argument concatenated together.
namespace detail {
	void concatAsString(std::string&);
	
	template <typename T, typename... Ts>
	void concatAsString(std::string& buf, const T& t, Ts&&... ts) {
		buf += toString(t);
		concatAsString(buf, std::forward<Ts>(ts)...);
	}
}

template <typename... Ts>
std::string concatAsString(Ts&&... ts) {
	std::string buf;
	detail::concatAsString(buf, std::forward<Ts>(ts)...);
	return buf;
}


} // ns stardazed

#endif
