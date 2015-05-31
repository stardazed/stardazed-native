// ------------------------------------------------------------------
// render::opengl::Pipeline.cpp - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "render/opengl/Pipeline.hpp"

namespace stardazed {
namespace render {


Pipeline::Pipeline(const PipelineDescriptor& descriptor)
: descriptor_(descriptor)
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


void Pipeline::bind() {
	glBindProgramPipeline(glPipeline_);
}


} // ns render
} // ns stardazed
