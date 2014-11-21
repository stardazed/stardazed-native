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


// -- GL binding helpers

// need to be specialized for specific gl types
template <typename GLObj>
void bind(const GLObj&);

template <typename GLObj>
void clearBinding(const GLObj&);


// --


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


template <typename Bindable>
class RecursiveBindable {
	const Bindable& bindable_;
	int bindCount_ = 0;
	
public:
	constexpr RecursiveBindable(const Bindable& bindable)
	: bindable_(bindable)
	{}
	
	~RecursiveBindable() {
		assert(bindCount_ == 0);
	}
	
	void bind() {
		if (bindCount_ == 0)
			bindable_.bind();
		++bindCount_;
	}
	
	void unbind() {
		assert(bindCount_ > 0);
		--bindCount_;
		if (bindCount_ == 0)
			bindable_.unbind();
	}
	
	int bindCount() const { return bindCount_; }
};


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
	
	GLuint glVAO() const { return vao_; }
};

template <>
void bind(const GLVertexArrayObject& vao) {
	glBindVertexArray(vao.glVAO());
}

template <>
void clearBinding(const GLVertexArrayObject&) {
	glBindVertexArray(0);
}


	
} // ns render
} // ns stardazed


#endif
