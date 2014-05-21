// ------------------------------------------------------------------
// render::OpenGLPipeline - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_OPENGLPIPELINE_H
#define SD_RENDER_OPENGLPIPELINE_H

#include "render/Pipeline.hpp"
#include "render/opengl/OpenGLShader.hpp"


namespace stardazed {
namespace render {


class OpenGLPipeline : public Pipeline<OpenGLShader> {
	GLuint glName_ = 0;
	
public:
	OpenGLPipeline();
	~OpenGLPipeline();
	
	void attachShader(OpenGLShader* s) override;
	
	void activate() override;
	void deactivate() override;
};



} // ns render
} // ns stardazed

#endif
