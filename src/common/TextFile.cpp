// ------------------------------------------------------------------
// TextFile.cpp - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "common/TextFile.hpp"
#include "system/Logging.hpp"

#include <fstream>
#include <vector>

namespace stardazed {


std::string readTextFile(const std::string & filePath) {
	std::ifstream file { filePath };
	if (! file.is_open()) {
		log("can't find file " + filePath);
		return "";
	}
	
	file.unsetf(std::ios::skipws);
	file.seekg(0, std::ios::end);
	auto fileSize = static_cast<size_t>(file.tellg());
	
	std::string contents;
	std::vector<char> buffer(fileSize);
	file.seekg(0);
	file.read(buffer.data(), fileSize);
	
	contents.assign(buffer.begin(), buffer.end());
	return contents;
}


} // ns stardazed
