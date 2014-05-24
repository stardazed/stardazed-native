// ------------------------------------------------------------------
// render::OpenGLContext - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_OPENGLCONTEXT_H
#define SD_RENDER_OPENGLCONTEXT_H

#include "render/Context.hpp"
#include "render/opengl/OpenGLShader.hpp"
#include "render/opengl/OpenGLPipeline.hpp"

#include <string>

namespace stardazed {
namespace render {


struct OpenGLContextTag {};


template<>
struct ContextTraits<OpenGLContextTag> {
	using ShaderClass = OpenGLShader;
	using PipelineClass = OpenGLPipeline;
};


class OpenGLContext : public Context<OpenGLContextTag> {
public:
	OpenGLContext(ContextOptions options);
	~OpenGLContext();
	
	friend Context;
	
	ShaderClass loadShaderFromPath(ShaderType type, const std::string& path) override;
	PipelineClass makePipeline() override;
	
	void swap() override;

private:
	class PlatformData;
 	std::unique_ptr<PlatformData> platformData_;
};


} // ns render
} // ns stardazed

#endif
