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

/*
template <typename Impl>
class Context {
	ContextOptions options;
	
protected:
	Context() = delete;
	
	Context(ContextOptions options)
	: options(options) {}
	
public:
	virtual ~Context() = default;
	
	template <ShaderType Type>
	typename Impl::template ShaderImpl<Type>* loadShaderFromPath(const std::string& path);
	
	typename Impl::PipelineImpl* makePipeline();
	
	void swap();
};
*/


class OpenGLContext : public Context<OpenGLContext> {
	std::string loadShaderFile(const std::string& path);
	
	class PlatformData;
	std::unique_ptr<PlatformData> platformData;

public:
	template <ShaderType Type>
	using ShaderImpl = OpenGLShader<Type>;
	using PipelineImpl = OpenGLPipeline;

	OpenGLContext(ContextOptions options);

	template <ShaderType Type>
	ShaderImpl<Type> loadShaderFromPathImpl(const std::string& path) {
		return ShaderImpl<Type>(loadShaderFile(path));
	}
	
	PipelineImpl makePipelineImpl();

	void swapImpl();
};


} // ns render
} // ns stardazed

#endif
