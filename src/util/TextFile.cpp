// ------------------------------------------------------------------
// TextFile.cpp - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "util/TextFile.hpp"
#include "filesystem/FileSystem.hpp"

namespace stardazed {


std::string readTextFile(const std::string& filePath) {
	fs::Path path { filePath };
	fs::FileReadStream file { path };
	auto fileSize = path.fileSize();
	
	std::string contents;
	contents.resize(fileSize);
	file.readBytes(&contents.front(), fileSize);
	
	return contents;
}


} // ns stardazed
