// ------------------------------------------------------------------
// render::OpenGLConstantBuffer - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_OPENGLCONSTANTBUFFER_H
#define SD_RENDER_OPENGLCONSTANTBUFFER_H

#include "render/opengl/OpenGL.hpp"
#include "render/ConstantBuffer.hpp"

namespace stardazed {
namespace render {
		

class OpenGLConstantBuffer : public ConstantBuffer {
	math::Mat4 modelViewMatrix_, modelViewProjectionMatrix_;
	math::Mat3 normalMatrix_;

public:
	void setModelViewMatrix(const math::Mat4&) override;
	void setModelViewProjectionMatrix(const math::Mat4&) override;
	void setNormalMatrix(const math::Mat3&) override;
	
	const math::Mat4& modelViewMatrix() const { return modelViewMatrix_; }
	const math::Mat4& modelViewProjectionMatrix() const { return modelViewProjectionMatrix_; }
	const math::Mat3& normalMatrix() const { return normalMatrix_; }
};
		
		
} // ns render
} // ns stardazed

#endif
