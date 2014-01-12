// ------------------------------------------------------------------
// Config - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_CONFIG_H
#define SD_CONFIG_H

#include <string>

namespace stardazed {

class Config {
public:
	std::string value(const std::string& keyPath, const std::string& defaultValue);
};

} // stardazed namespace

#endif
