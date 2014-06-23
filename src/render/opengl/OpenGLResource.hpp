// ------------------------------------------------------------------
// render::OpenGLResource - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_OPENGLRESOURCE_H
#define SD_RENDER_OPENGLRESOURCE_H

#include "render/opengl/OpenGL.hpp"
#include "util/ConceptTraits.hpp"

namespace stardazed {
namespace render {


// GLName is a move-only container type for OpenGL "names"
// Given that names are a unique reference to a resource that
// needs to be disposed of, this guards the name and ensures
// it can only be moved, not casually copied.

template <typename Deleter>
class GLResource {
	GLuint name_ = 0;
	
	using DeleterType = Deleter;
	
	void release() {
		if (name_ == 0)
			return;
		(DeleterType())(name_);
		name_ = 0;
	}
	
public:
	constexpr GLResource() = default;
	explicit constexpr GLResource(GLuint name) : name_{name} {}
	
	GLResource(GLResource&& from)
	: name_{ from.name_ }
	{ from.name_ = 0; }
	
	GLResource& operator=(GLResource&& from) {
		release();
  		name_ = from.name_;
		from.name_ = 0;
		return *this;
	}
	
	~GLResource() { release(); }
	
	constexpr GLuint& name() { return name_; }
	constexpr GLuint name() const { return name_; }
	
	constexpr operator bool() const { return name_ != 0; }
	constexpr operator GLuint() const { return name_; }
};


struct GLShaderDeleter { void operator()(GLuint name) { glDeleteProgram(name); } };

//using GLShaderResource = GLResource


} // ns render
} // ns stardazed

#endif


