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


// -- bind multiple objects

template <typename GLObj>
inline void bindMultiple(const GLObj& obj) {
	obj.bind();
}

template <typename GLObj, typename... More>
inline void bindMultiple(const GLObj& obj, More&&... more) {
	obj.bind();
	bindMultiple(std::forward<More&&>(more)...);
}


// -- specialized by bindable types

template <typename GLObj>
GLuint saveAndBind(const GLObj&);

template <typename GLObj>
void unbindAndRestore(const GLObj&, GLuint previousBinding);


// -- do action in lambda with one or more objects bound
// -- the previous bindings are restored after the lambda

template <typename F>
void withTempBound(F&& func) {
	func();
}

template <typename GLObj, typename... More>
void withTempBound(const GLObj& obj, More&&... more) {
	auto previous = saveAndBind(obj);
	withTempBound(std::forward<More&&>(more)...);
	unbindAndRestore(obj, previous);
}


} // ns render
} // ns stardazed


#endif
