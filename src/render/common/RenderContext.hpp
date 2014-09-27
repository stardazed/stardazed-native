// ------------------------------------------------------------------
// render::RenderContext - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_RENDERCONTEXT_H
#define SD_RENDER_RENDERCONTEXT_H

#include "system/Config.hpp"
#include "util/ConceptTraits.hpp"
#include "render/common/Mesh.hpp"
#include "render/common/Shader.hpp"
#include "render/common/Pipeline.hpp"

#include <string>
#include <memory>

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


struct RenderContextDescriptor {
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
