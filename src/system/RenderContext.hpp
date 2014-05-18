// ------------------------------------------------------------------
// system::RenderContext - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_SYSTEM_RENDERCONTEXT_H
#define SD_SYSTEM_RENDERCONTEXT_H

#include "system/RenderContextOptions.hpp"
#include "render/Shader.hpp"
#include "render/Pipeline.hpp"

#include <string>
#include <memory>

namespace stardazed {


class RenderContext {
	RenderContextOptions options;
	
	class Impl;
	std::unique_ptr<Impl> pimpl;

public:
	RenderContext(RenderContextOptions options);
	~RenderContext();

	render::VertexShaderRef makeVertexShaderFromPath(const std::string& path);
	render::TesselationControlShaderRef makeTesselationControlShaderFromPath(const std::string& path);
	render::TesselationEvalShaderRef makeTesselationEvalShaderFromPath(const std::string& path);
	render::GeometryShaderRef makeGeometryShaderFromPath(const std::string& path);
	render::FragmentShaderRef makeFragmentShaderFromPath(const std::string& path);

	render::PipelineRef makeRenderPipeline();

	void swap();
};


} // ns stardazed

#endif
