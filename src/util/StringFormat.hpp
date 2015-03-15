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



// Simple printf-like formatted string -- experimental
// FIXME: needs actual argument inspection
class FmtString {
	std::string fmt_, baked_;
	std::string::const_iterator scan_;
	
	void appendValue(const std::string&);
	
public:
	FmtString(std::string s);
	FmtString(const char* cs) : FmtString(std::string{cs}) {}
	
	const std::string& str() const {
		return baked_;
	}
	
	operator std::string() {
		return baked_;
	}
	
	template <typename T>
	FmtString& operator %(T t) {
		// FIXME: check type of placeholder, etc.
		appendValue(toString(t));
		return *this;
	}
};



} // ns stardazed

#endif
