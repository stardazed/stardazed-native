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
};


} // ns render
} // ns stardazed

#endif
