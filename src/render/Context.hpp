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


template <typename Impl>
class Context {
protected:
	ContextOptions options;

	Context() = delete;

	Context(ContextOptions options)
	: options(options) {}

public:
	virtual ~Context() = default;
	
	using ShaderClass = typename ContextTraits<Impl>::ShaderClass;
	using PipelineClass = typename ContextTraits<Impl>::PipelineClass;

	ShaderClass loadShaderFromPath(ShaderType type, const std::string& path) {
		return static_cast<Impl*>(this)->loadShaderFromPathImpl(type, path);
	}
	
	PipelineClass makePipeline() {
		return static_cast<Impl*>(this)->makePipelineImpl();
	}

	void swap() {
		return static_cast<Impl*>(this)->swapImpl();
	}
};


} // ns render
} // ns stardazed

#endif
