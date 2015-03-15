// ------------------------------------------------------------------
// system::Logging - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_SYSTEM_LOGGING_H
#define SD_SYSTEM_LOGGING_H

#include "system/Config.hpp"
#include "util/StringFormat.hpp"

#include <string>

namespace stardazed {

	
void log(const char*);

inline void log(const std::string& msg) {
	log(msg.c_str());
}

template <typename... Ts>
inline void log(Ts&&... ts) {
	auto str = concatAsString(std::forward<Ts>(ts)...);
	log(str.c_str());
}


} // ns stardazed

#endif
