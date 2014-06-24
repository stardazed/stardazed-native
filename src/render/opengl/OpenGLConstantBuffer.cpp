// ------------------------------------------------------------------
// render::OpenGLConstantBuffer - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "render/opengl/OpenGLConstantBuffer.hpp"

namespace stardazed {
namespace render {
	

void OpenGLConstantBuffer::setModelViewMatrix(const math::Mat4& mv) {
	modelViewMatrix_ = mv;
}


void OpenGLConstantBuffer::setModelViewProjectionMatrix(const math::Mat4& mvp) {
	modelViewProjectionMatrix_ = mvp;
}


void OpenGLConstantBuffer::setNormalMatrix(const math::Mat3& nm) {
	normalMatrix_ = nm;
}

	
} // ns render
} // ns stardazed
