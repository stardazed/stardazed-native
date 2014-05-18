// ------------------------------------------------------------------
// render::OpenGLPipeline - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_OPENGLPIPELINE_H
#define SD_RENDER_OPENGLPIPELINE_H

#include "render/Pipeline.hpp"
#include "render/OpenGL.hpp"


namespace stardazed {
namespace render {


class OpenGLPipeline : public Pipeline {
	GLuint pipelineName = 0;
	
public:
	OpenGLPipeline();
	~OpenGLPipeline();
	
	void setVertexStage(const VertexShaderRef&) override;
	void setTesselationControlStage(const TesselationControlShaderRef&) override;
	void setTesselationEvalStage(const TesselationEvalShaderRef&) override;
	void setGeometryStage(const GeometryShaderRef&) override;
	void setFragmentStage(const FragmentShaderRef&) override;
	
	void activate() override;
	void deactivate() override;
};



} // ns render
} // ns stardazed

#endif
