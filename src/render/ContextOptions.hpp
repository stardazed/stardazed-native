// ------------------------------------------------------------------
// render::ContextOptions - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_CONTEXTOPTIONS_H
#define SD_RENDER_CONTEXTOPTIONS_H

namespace stardazed {
namespace render {


enum class FullscreenAntiAliasMethod {
	None,
	SSAA,
	MSAA
};


enum class BufferingMethod {
	DoubleBuffer,
	TripleBuffer
};


struct ContextOptions {
	int	width = 1024, height = 768;
	bool fullscreen = false;
	bool verticalSync = false;
	BufferingMethod bufferMethod = BufferingMethod::DoubleBuffer;

	FullscreenAntiAliasMethod fsaa = FullscreenAntiAliasMethod::MSAA;
	int antiAliasSamples = 4;

	// buffer sizes for non-colour buffers
	// the colour buffer is fixed at 24+8 bits
	int depthBits = 0;
	int stencilBits = 0;
	int accumulatorBits = 0;
	int auxBufferCount = 0;
};


} // ns render
} // ns stardazed

#endif
