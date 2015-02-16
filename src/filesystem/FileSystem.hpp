// ------------------------------------------------------------------
// fs::FileSystem - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_FS_FILESYSTEM_H
#define SD_FS_FILESYSTEM_H

#include "system/Config.hpp"

#include <string>
#include <CoreFoundation/CFURL.h>
#include <CoreFoundation/CFStream.h>

namespace stardazed {
namespace fs {


class Path {
	CFURLRef url_;

public:
	explicit Path(const std::string& absPath);
	explicit Path(const char* absPath) : Path(std::string{ absPath }) {}
	
	Path(const Path& basePath, const std::string& relPath);
	Path(const Path& basePath, const char* relPath) : Path(basePath, std::string{ relPath }) {}
	
	Path(const Path&);
	
	~Path();
	
	CFURLRef nativeHandle() const { return url_; }
	
	std::string toString() const;
	
	int64 fileSize() const;
};


class FileReadStream {
	CFReadStreamRef stream_;

public:
	FileReadStream(const Path&);
	~FileReadStream();
	
	void readBytes(void* buffer, size64 byteCount);
	
	template <typename T>
	void readStruct(T* object) {
		readBytes(object, sizeof(T));
	}
	
	int64 offset() const;
	void seekAbsolute(int64 newOffset) const;
	void seekRelative(int64 displacement) const;
};

	
} // ns fs
} // ns stardazed

#endif
