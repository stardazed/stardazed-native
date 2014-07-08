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
	int	width = 1280, height = 720; // 16:9
	bool fullscreen = false;
	bool verticalSync = false;
	BufferingMethod bufferMethod = BufferingMethod::DoubleBuffer;

	FullscreenAntiAliasMethod fsaa = FullscreenAntiAliasMethod::MSAA;
	int antiAliasSamples = 4;
	
	int depthBits = 0; // default depth buffer size in bits/sample (0, 16, 32)
};


} // ns render
} // ns stardazed

#endif
