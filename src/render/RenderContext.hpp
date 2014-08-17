// ------------------------------------------------------------------
// render::RenderContext - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_RENDERCONTEXT_H
#define SD_RENDER_RENDERCONTEXT_H

#include "render/Mesh.hpp"
#include "render/Shader.hpp"
#include "render/Pipeline.hpp"

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


class RenderContext {
public:
	virtual ~RenderContext() = default;

	// factory methods
	virtual Mesh* makeStaticMesh(const MeshDescriptor&) = 0;
	virtual Shader* loadShaderFromPath(ShaderType type, const std::string& path) = 0;
	virtual Pipeline* makePipeline(const PipelineDescriptor&) = 0;

	// action methods
	virtual void swap() = 0;
};


} // ns render
} // ns stardazed

#endif
