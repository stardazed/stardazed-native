// ------------------------------------------------------------------
// render::OpenGLPipeline - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_OPENGLPIPELINE_H
#define SD_RENDER_OPENGLPIPELINE_H

#include "render/Pipeline.hpp"
#include "render/opengl/OpenGLShader.hpp"
#include "util/ConceptTraits.hpp"


namespace stardazed {
namespace render {


class OpenGLPipeline : public Pipeline<OpenGLShader> {
	GLName glPipeline_;
	
public:
	OpenGLPipeline();
	~OpenGLPipeline();
	SD_DEFAULT_MOVE_OPS(OpenGLPipeline)
	
	void attachShader(OpenGLShader* s) override;
	
	void activate() override;
	void deactivate() override;
};


} // ns render
} // ns stardazed

#endif
