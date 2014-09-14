// ------------------------------------------------------------------
// render::mac_OpenGLContext - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_MAC_OPENGLCONTEXT_H
#define SD_RENDER_MAC_OPENGLCONTEXT_H

#include "system/Config.hpp"
#include "container/ObjectPool.hpp"
#include "render/RenderContext.hpp"
#include "render/opengl/OpenGLMesh.hpp"
#include "render/opengl/OpenGLShader.hpp"
#include "render/opengl/OpenGLPipeline.hpp"

#include <string>
#include <memory>

namespace stardazed {
namespace render {


class OpenGLContext : public RenderContext {
public:
	OpenGLContext(const RenderContextDescriptor&);
	~OpenGLContext();
	
	Mesh* makeStaticMesh(const MeshDescriptor&) override;
	Shader* loadShaderFromPath(ShaderType type, const std::string& path) override;
	Pipeline* makePipeline(const PipelineDescriptor& descriptor) override;
	
	void swap() override;
	
	bool isFullscreen() const override;
	bool usesVerticalSync() const override;


private:
	// object storage
	container::ObjectPool<OpenGLPipeline, 128> pipelinePool_;
	container::ObjectPool<OpenGLShader, 512> shaderPool_;
	container::ObjectPoolChain<OpenGLMesh, 512> meshPool_;

	// place obj-c stuff in implementation file only
	class PlatformData;
 	std::unique_ptr<PlatformData> platformData_;
};


} // ns render
} // ns stardazed

#endif
