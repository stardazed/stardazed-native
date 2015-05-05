// ------------------------------------------------------------------
// render::PixelBuffer - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_PIXELBUFFER_H
#define SD_RENDER_PIXELBUFFER_H

#include "system/Config.hpp"
#include "render/common/PixelFormat.hpp"

namespace stardazed {
namespace render {


struct PixelBuffer {
	void* data;
	PixelFormat format;
	size32 width, height, sizeBytes;
};


} // ns render
} // ns stardazed

#endif
