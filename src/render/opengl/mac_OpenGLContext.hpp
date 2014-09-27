// ------------------------------------------------------------------
// render::mac_OpenGLContext - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_MAC_OPENGLCONTEXT_H
#define SD_RENDER_MAC_OPENGLCONTEXT_H

#include "system/Config.hpp"
#include "container/ObjectPool.hpp"
#include "render/common/RenderContext.hpp"
#include "render/opengl/OpenGLMesh.hpp"
#include "render/opengl/OpenGLShader.hpp"
#include "render/opengl/OpenGLPipeline.hpp"

#include <string>
#include <memory>

namespace stardazed {
namespace render {


class RenderContext {
public:
	RenderContext(const RenderContextDescriptor&);
	~RenderContext();
	
	Mesh* makeStaticMesh(const MeshDescriptor&);
	Shader* loadShaderFromPath(ShaderType type, const std::string& path);
	Pipeline* makePipeline(const PipelineDescriptor& descriptor);
	
	void swap();
	
	bool isFullscreen() const;
	bool usesVerticalSync() const;

private:
	// object storage
	container::ObjectPool<Pipeline, 128> pipelinePool_;
	container::ObjectPool<Shader, 512> shaderPool_;
	container::ObjectPoolChain<Mesh, 512> meshPool_;

	// place obj-c stuff in implementation file only
	class PlatformData;
 	std::unique_ptr<PlatformData> platformData_;
};


} // ns render
} // ns stardazed

#endif
