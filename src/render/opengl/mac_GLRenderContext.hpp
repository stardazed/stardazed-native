// ------------------------------------------------------------------
// render::mac_GLRenderContext - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_MAC_GLRENDERCONTEXT_H
#define SD_RENDER_MAC_GLRENDERCONTEXT_H

#include "system/Config.hpp"
#include "render/common/RenderContext.hpp"
#include "render/opengl/Shader.hpp"
#include "render/opengl/Pipeline.hpp"

#include <vector>
#include <string>
#include <memory>

namespace stardazed {
namespace render {


class RenderContext {
public:
	RenderContext(const RenderContextDescriptor&);
	~RenderContext();
	
	Shader* loadShaderFromPath(ShaderType type, const std::string& path);
	Program* makeShaderProgram(Shader&);
	Pipeline* makePipeline(const PipelineDescriptor& descriptor);
	Pipeline* makePipeline(const SSOPipelineDescriptor& descriptor);
	
	void swap();
	
	bool isFullscreen() const;
	bool usesVerticalSync() const;

	uint32 pixelWidth() const { return width_; }
	uint32 pixelHeight() const { return height_; }

private:
	uint32 width_, height_;

	// object storage
	std::vector<Pipeline> pipelinePool_;
	std::vector<Shader> shaderPool_;
	std::vector<Program> programPool_;

	// place obj-c stuff in implementation file only
	class PlatformData;
 	std::unique_ptr<PlatformData> platformData_;
};


} // ns render
} // ns stardazed

#endif
