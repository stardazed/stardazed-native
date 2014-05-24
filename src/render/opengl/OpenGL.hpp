// ------------------------------------------------------------------
// render::OpenGL - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_OPENGL_H
#define SD_RENDER_OPENGL_H

#include "system/DetectPlatform.hpp"

// include platform-specific OpenGL headers
#if SD_PLATFORM_OSX
#	include <OpenGL/gl3.h>
#	include <OpenGL/gl3ext.h>
#else
#	error "No OpenGL support yet for this platform"
#endif


// GLName is a move-only container type for OpenGL "names"
// Given that names are a unique reference to a resource that
// needs to be disposed of, this guards the name and ensures
// it can only be moved, not casually copied.

class GLName {
	GLuint name_ = 0;

public:
	constexpr GLName() = default;
	explicit constexpr GLName(GLuint name) : name_{name} {}
	
	GLName(GLName&& from)
	: name_{ from.name_ }
	{ from.name_ = 0; }
	
	GLName& operator=(GLName&& from) {
		name_ = from.name_;
		from.name_ = 0;
		return *this;
	}
	
	constexpr GLuint& name() { return name_; }
	constexpr GLuint name() const { return name_; }
	
	constexpr operator bool() const { return name_ != 0; }
	constexpr operator GLuint() const { return name_; }
};


#endif
