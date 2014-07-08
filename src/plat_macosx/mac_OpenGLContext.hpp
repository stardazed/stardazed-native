// ------------------------------------------------------------------
// render::mac_OpenGLContext - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_MAC_OPENGLCONTEXT_H
#define SD_RENDER_MAC_OPENGLCONTEXT_H

#include "render/Context.hpp"
#include "render/opengl/OpenGLMesh.hpp"
#include "render/opengl/OpenGLShader.hpp"
#include "render/opengl/OpenGLPipeline.hpp"

#include <string>
#include <memory>

namespace stardazed {
namespace render {


struct MacOpenGLContextTag {};


template<>
struct ContextTraits<MacOpenGLContextTag> {
	using MeshClass = OpenGLMesh;
	using ShaderClass = OpenGLShader;
	using PipelineClass = OpenGLPipeline;
};


class OpenGLContext : public Context<MacOpenGLContextTag> {
public:
	OpenGLContext(const ContextDescriptor&);
	~OpenGLContext();
	
	MeshClass makeStaticMesh(const Mesh&) override;
	ShaderClass loadShaderFromPath(ShaderType type, const std::string& path) override;
	PipelineClass makePipeline(const OpenGLPipeline::Descriptor& descriptor) override;
	
	void swap() override;

private:
	// place obj-c stuff in implementation file only
	class PlatformData;
 	std::unique_ptr<PlatformData> platformData_;
};


} // ns render
} // ns stardazed

#endif
