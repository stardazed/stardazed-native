// ------------------------------------------------------------------
// render::opengl::Pipeline.cpp - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "render/opengl/Pipeline.hpp"

namespace stardazed {
namespace render {


Pipeline::Pipeline(const PipelineDescriptor& descriptor)
: descriptor_(descriptor)
, vertexLayout_(descriptor.vertexAttributes)
{
	glGenProgramPipelines(1, &glPipeline_);
	
	if (vertexShader()) {
		glUseProgramStages(glPipeline_, GL_VERTEX_SHADER_BIT, vertexShader()->name());
	}

	if (geometryShader()) {
		glUseProgramStages(glPipeline_, GL_GEOMETRY_SHADER_BIT, geometryShader()->name());
	}
	
	if (fragmentShader()) {
		glUseProgramStages(glPipeline_, GL_FRAGMENT_SHADER_BIT, fragmentShader()->name());
	}
}


Pipeline::~Pipeline() {
	if (glPipeline_ > 0)
		glDeleteProgramPipelines(1, &glPipeline_);
}


void Pipeline::bind() const {
	glBindProgramPipeline(glPipeline_);
	if (! descriptor_.rasterizerEnabled)
		glEnable(GL_RASTERIZER_DISCARD);
	
	// FIXME: set all the blending modes, etc
}


void Pipeline::unbind() const {
	glBindProgramPipeline(0);
	if (! descriptor_.rasterizerEnabled)
		glDisable(GL_RASTERIZER_DISCARD);
}


} // ns render
} // ns stardazed
