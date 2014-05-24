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


template<>
struct ContextTraits<class OpenGLContext> {
	using ShaderClass = OpenGLShader;
	using PipelineClass = OpenGLPipeline;
};


class OpenGLContext : public Context<OpenGLContext> {
public:
	OpenGLContext(ContextOptions options);
	~OpenGLContext();
	
	friend Context;

private:
	class PlatformData;
 	std::unique_ptr<PlatformData> platformData_;

	ShaderClass loadShaderFromPathImpl(ShaderType type, const std::string& path);
	
	PipelineClass makePipelineImpl();
	
	void swapImpl();
};


} // ns render
} // ns stardazed

#endif
