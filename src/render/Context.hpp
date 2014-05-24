// ------------------------------------------------------------------
// render::Context - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_CONTEXT_H
#define SD_RENDER_CONTEXT_H

#include "render/ContextOptions.hpp"
#include "render/Shader.hpp"
#include "render/Pipeline.hpp"

#include <string>
#include <memory>

namespace stardazed {
namespace render {


template <typename T>
struct ContextTraits;


template <typename Tag>
class Context {
protected:
	ContextOptions options;

	Context(ContextOptions options)
	: options(options) {}

public:
	virtual ~Context() = default;

	// mirror trait classes for easy access
	using ShaderClass = typename ContextTraits<Tag>::ShaderClass;
	using PipelineClass = typename ContextTraits<Tag>::PipelineClass;

	// factory methods
	virtual ShaderClass loadShaderFromPath(ShaderType type, const std::string& path) = 0;
	virtual PipelineClass makePipeline() = 0;

	// action methods
	virtual void swap() = 0;
};


} // ns render
} // ns stardazed

#endif
