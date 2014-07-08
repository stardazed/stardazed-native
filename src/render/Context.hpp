// ------------------------------------------------------------------
// render::Context - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_CONTEXT_H
#define SD_RENDER_CONTEXT_H

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


struct ContextDescriptor {
	int	width = 1280, height = 720; // 16:9
	bool fullscreen = false;
	bool verticalSync = false;
	BufferingMethod bufferMethod = BufferingMethod::DoubleBuffer;
	
	FullscreenAntiAliasMethod fsaa = FullscreenAntiAliasMethod::MSAA;
	int antiAliasSamples = 4;
	
	int depthBits = 0; // default depth buffer size in bits/sample (0, 16, 32)
};


template <typename T>
struct ContextTraits;
/*
	type MeshClass
	type ShaderClass
	type PipelineClass
*/


template <typename Tag>
class Context {
public:
	virtual ~Context() = default;

	// mirror trait classes for easy access
	using MeshClass = typename ContextTraits<Tag>::MeshClass;
	using ShaderClass = typename ContextTraits<Tag>::ShaderClass;
	using PipelineClass = typename ContextTraits<Tag>::PipelineClass;

	// factory methods
	virtual MeshClass makeStaticMesh(const Mesh&) = 0;
	virtual ShaderClass loadShaderFromPath(ShaderType type, const std::string& path) = 0;
	virtual PipelineClass makePipeline(const typename PipelineClass::Descriptor&) = 0;

	// action methods
	virtual void swap() = 0;
};


} // ns render
} // ns stardazed

#endif
