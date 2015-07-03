// ------------------------------------------------------------------
// render::common::Buffer - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_BUFFER_H
#define SD_RENDER_BUFFER_H

#include "system/Config.hpp"


namespace stardazed {
namespace render {


enum class BufferRole {
	VertexAttribute,
	VertexIndex,
	TextureBuffer,
	TransformFeedback,
	ConstantBuffer
};


class Buffer;


} // ns render
} // ns stardazed

#endif
