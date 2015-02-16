// ------------------------------------------------------------------
// StringFormat - stardazed
// (c) 2014 by Arthur Langereis
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
