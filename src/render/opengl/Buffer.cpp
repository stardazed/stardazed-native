// ------------------------------------------------------------------
// render::opengl::Buffer - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "render/opengl/Buffer.hpp"
#include "render/common/VertexBuffer.hpp"

namespace stardazed {
namespace render {
		

namespace detail {
	template <GLenum target>
	GLint IndexedBufferOps<target>::maxIndex_ = -1;
}


} // ns render
} // ns stardazed
