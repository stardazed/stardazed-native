// ------------------------------------------------------------------
// render::opengl::OpenGL - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_OPENGL_H
#define SD_RENDER_OPENGL_H

#include "system/Config.hpp"

// include platform-specific OpenGL headers
#if SD_PLATFORM_OSX
#	include <OpenGL/gl3.h>
#	include <OpenGL/gl3ext.h>
#else
#	error "No OpenGL support yet for this platform"
#endif

#include <type_traits>


namespace stardazed {
namespace render {


// -- bind single object

template <typename GLObj>
void bind(const GLObj&);

template <typename GLObj>
inline void bind(GLObj* obj) {
	bind(*obj);
}



class GLResource {
	GLuint name_ = 0;
public:
	constexpr GLResource() = default;
	constexpr GLResource(GLuint name)
	: name_(name)
	{}

#ifndef NDEBUG
	~GLResource() {
		// to use a GLResource, you must have clear()ed in
		// your own destructor explicitly
		assert(name_ == 0);
	}
#endif

	GLResource(GLResource&& other) noexcept
	: name_(other.name_)
	{
		other.name_ = 0;
	}

	GLResource& operator=(GLResource&& other) noexcept {
		assign(other.name_);
		other.name_ = 0;
		return *this;
	}

	void assign(GLuint newName) noexcept {
		// GLResource only keeps track of a name, not a deleter
		// so you cannot overwrite an existing name
		assert(name_ == 0);
		assert(newName != 0);

		name_ = newName;
	}
	
	void clear() {
		name_ = 0;
	}

#ifdef NDEBUG
	constexpr
#endif
	GLuint name() const { return name_; }
};



class OpenGL {
	OpenGL() = delete;

public:
	static bool hasExtension(const char*);
};


} // ns render
} // ns stardazed


#endif
