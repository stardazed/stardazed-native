// ------------------------------------------------------------------
// render::opengl::ConstantBuffer - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_OPENGLCONSTANTBUFFER_H
#define SD_RENDER_OPENGLCONSTANTBUFFER_H

#include "system/Config.hpp"
#include "math/Matrix.hpp"
#include "render/common/ConstantBuffer.hpp"
#include "render/opengl/OpenGL.hpp"

namespace stardazed {
namespace render {
		

class ConstantBuffer {
	GLuint glBuf_;

public:
	ConstantBuffer();
	~ConstantBuffer();
};
		
		
} // ns render
} // ns stardazed

#endif
