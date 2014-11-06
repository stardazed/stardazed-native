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


// -- basic GL "object" wrappers

class GLVertexArrayObject {
	GLuint vao_ = 0;
	
public:
	GLVertexArrayObject() {
		glGenVertexArrays(1, &vao_);
	}
	
	~GLVertexArrayObject() {
		if (vao_)
			glDeleteVertexArrays(1, &vao_);
	}
	
	void bind() const { glBindVertexArray(vao_); }
	void unbind() const { glBindVertexArray(0); }
};



// -- GL convenience functions

template <typename F>
inline void withBound(F&& func) {
	func();
}

template <typename GLObj, typename... More>
inline void withBound(const GLObj& obj, More&&... more) {
	obj.bind();
	withBound(std::forward<More&&>(more)...);
	obj.unbind();
}

	
} // ns render
} // ns stardazed


#endif
