// ------------------------------------------------------------------
// render::opengl::ConstantBuffer - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "render/opengl/ConstantBuffer.hpp"

namespace stardazed {
namespace render {
	

ConstantBuffer::ConstantBuffer() {
	glGenBuffers(1, &glBuf_);
}


ConstantBuffer::~ConstantBuffer() {
	glDeleteBuffers(1, &glBuf_);
}

	
} // ns render
} // ns stardazed
