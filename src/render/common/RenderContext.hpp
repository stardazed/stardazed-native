// ------------------------------------------------------------------
// render::RenderContext - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_RENDERCONTEXT_H
#define SD_RENDER_RENDERCONTEXT_H

#include "system/Config.hpp"

namespace stardazed {
namespace render {


enum class BufferingMethod {
	DoubleBuffer,
	TripleBuffer
};


struct RenderContextDescriptor {
	int	width = 1280, height = 720;
	bool fullscreen = false;
	bool verticalSync = false;
	BufferingMethod bufferMethod = BufferingMethod::DoubleBuffer;
	// colour buffers have a fixed 24-8 pixel format
};


} // ns render
} // ns stardazed

#endif
