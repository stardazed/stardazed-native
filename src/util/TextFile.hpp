// ------------------------------------------------------------------
// TextFile - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_TEXTFILE_H
#define SD_TEXTFILE_H

#include "system/Config.hpp"
#include "filesystem/FileSystem.hpp"

#include <string>

namespace stardazed {


std::string readTextFile(const fs::Path path);


} // ns stardazed

#endif
