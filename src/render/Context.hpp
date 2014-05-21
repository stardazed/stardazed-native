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


template <typename Impl>
class Context {
protected:
	ContextOptions options;

	Context() = delete;

	Context(ContextOptions options)
	: options(options) {}

public:
	virtual ~Context() = default;

	typename Impl::ShaderClass loadShaderFromPath(ShaderType type, const std::string& path) {
		return static_cast<Impl*>(this)->loadShaderFromPathImpl(type, path);
	}
	
	typename Impl::PipelineClass makePipeline() {
		return static_cast<Impl*>(this)->makePipelineImpl();
	}

	void swap() {
		return static_cast<Impl*>(this)->swapImpl();
	}
};


} // ns render
} // ns stardazed

#endif
