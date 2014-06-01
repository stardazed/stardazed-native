// ------------------------------------------------------------------
// system::Logging - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_SYSTEM_LOGGING_H
#define SD_SYSTEM_LOGGING_H

#include <string>
#include "util/StringFormat.hpp"

namespace stardazed {

	
void log(const char*);

inline void log(const std::string& msg) {
	log(msg.c_str());
}

inline void log(const FmtString& fs) {
	log(fs.str().c_str());
}

template <typename T, typename... Ts>
inline void log(FmtString& fs, T&& t, Ts&&... ts) {
	fs % t;
	log(fs, std::forward<Ts>(ts)...);
}

template <typename... Ts>
inline void log(const char* cs, Ts&&... ts) {
	FmtString fs{cs};
	log(fs, std::forward<Ts>(ts)...);
}

template <typename... Ts>
inline void log(const std::string& s, Ts&&... ts) {
	FmtString fs{s};
	log(fs, std::forward<Ts>(ts)...);
}


} // ns stardazed

#endif
