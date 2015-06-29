// ------------------------------------------------------------------
// TextFile.cpp - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "util/TextFile.hpp"

namespace stardazed {


std::string readTextFile(fs::Path path) {
	fs::FileReadStream file { path };
	auto fileSize = path.fileSize();
	
	std::string contents;
	contents.resize(fileSize);
	file.readBytes(&contents.front(), fileSize);
	
	return contents;
}


} // ns stardazed
