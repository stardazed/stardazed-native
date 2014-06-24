// ------------------------------------------------------------------
// render::ConstantBuffer - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_CONSTANTBUFFER_H
#define SD_RENDER_CONSTANTBUFFER_H

#include "math/Matrix.hpp"

namespace stardazed {
namespace render {


class ConstantBuffer {
public:
	virtual ~ConstantBuffer() = default;
	
	virtual void setModelViewMatrix(const math::Mat4&) = 0;
	virtual void setModelViewProjectionMatrix(const math::Mat4&) = 0;
	virtual void setNormalMatrix(const math::Mat3&) = 0;
};


} // ns render
} // ns stardazed

#endif
