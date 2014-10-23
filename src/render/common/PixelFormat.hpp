// ------------------------------------------------------------------
// render::PixelFormat - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_PIXELFORMAT_H
#define SD_RENDER_PIXELFORMAT_H

#include "system/Config.hpp"

#include <vector>

namespace stardazed {
namespace render {


enum class PixelFormat {
	None,

	// basic formats
	RGBA_8888,
	ABGR_8888
};


struct TextureDescriptor {
	PixelFormat format;
};


} // ns render
} // ns stardazed

#endif
