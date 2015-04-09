// ------------------------------------------------------------------
// fs::FileSystem - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#include "filesystem/FileSystem.hpp"

#include <algorithm>

#include <CoreFoundation/CoreFoundation.h>

namespace stardazed {
namespace fs {


Path::Path(const std::string& absPath)
: url_(CFURLCreateFromFileSystemRepresentation(nullptr, (uint8*)absPath.data(), absPath.size(), false))
{}


Path::Path(const Path& basePath, const std::string& relPath)
: url_(CFURLCreateFromFileSystemRepresentationRelativeToBase(nullptr, (uint8*)relPath.data(), relPath.size(), false, basePath.nativeHandle()))
{}


Path::Path(const Path& path)
: url_(path.url_)
{
	CFRetain(url_);
}


Path::~Path() {
	CFRelease(url_);
}


int64 Path::fileSize() const {
	CFNumberRef fileSizeNr;
	int64 fileSize = 0;

	if (CFURLCopyResourcePropertyForKey(url_, kCFURLFileSizeKey, &fileSizeNr, nullptr)) {
		CFNumberGetValue(fileSizeNr, kCFNumberLongLongType, &fileSize);
	}
	
	return fileSize;
}


void getCFStringString(CFStringRef cfStr, std::string& intoString) {
	auto strPtr = CFStringGetCStringPtr(cfStr, kCFStringEncodingUTF8);
	if (strPtr) {
		// fast path, we have a direct char*
		auto byteCount = std::strlen(strPtr);
		intoString.resize(byteCount);
		std::copy(strPtr, strPtr + byteCount, intoString.begin());
	}
	else {
		// slow path, copy into oversized buffer
		auto maxLen = CFStringGetMaximumSizeForEncoding(CFStringGetLength(cfStr), kCFStringEncodingUTF8);
		intoString.resize(maxLen + 1);
		CFStringGetCString(cfStr, &intoString.front(), maxLen, kCFStringEncodingUTF8);
		intoString.resize(strlen(intoString.c_str()));
	}
}


std::string Path::toString() const {
	std::string path;

	auto absURL = CFURLCopyAbsoluteURL(url_);
	auto pathString = CFURLCopyFileSystemPath(absURL, kCFURLPOSIXPathStyle);

	getCFStringString(pathString, path);

	CFRelease(pathString);
	CFRelease(absURL);

	return path;
}


std::string Path::extension() const {
	std::string ext{};
	auto cfExt = CFURLCopyPathExtension(url_);
	if (cfExt) {
		getCFStringString(cfExt, ext);

		// force lower-case extension
		std::transform(ext.begin(), ext.end(), ext.begin(), std::tolower);
		
		CFRelease(cfExt);
	}
	return ext;
}


FileReadStream::FileReadStream(const Path& path)
: stream_(CFReadStreamCreateWithFile(nullptr, path.nativeHandle()))
{
	bool open = CFReadStreamOpen(stream_);
	assert(open);
}


FileReadStream::~FileReadStream() {
	CFReadStreamClose(stream_);
	CFRelease(stream_);
}


void FileReadStream::readBytes(void* buffer, size64 byteCount) {
	CFReadStreamRead(stream_, static_cast<uint8*>(buffer), byteCount);
}


int64 FileReadStream::offset() const {
	auto offsetNr = static_cast<CFNumberRef>(CFReadStreamCopyProperty(stream_, kCFStreamPropertyFileCurrentOffset));
	size64 offset = 0;
	CFNumberGetValue(offsetNr, kCFNumberLongLongType, &offset);
	CFRelease(offsetNr);
	return offset;
}


void FileReadStream::seekAbsolute(int64 newOffset) const {
	auto offsetNr = CFNumberCreate(nullptr, kCFNumberLongLongType, &newOffset);
	CFReadStreamSetProperty(stream_, kCFStreamPropertyFileCurrentOffset, offsetNr);
	CFRelease(offsetNr);
}


void FileReadStream::seekRelative(int64 displacement) const {
	seekAbsolute(offset() + displacement);
}


bool FileReadStream::eof() const {
	return CFReadStreamGetStatus(stream_) == kCFStreamStatusAtEnd;
}


bool FileReadStream::ok() const {
	auto status = CFReadStreamGetStatus(stream_);
	return status != kCFStreamStatusAtEnd && status != kCFStreamStatusError;
}



} // ns fs
} // ns stardazed
