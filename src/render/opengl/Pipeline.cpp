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
	GLuint pipelineName = 0;
	glGenProgramPipelines(1, &pipelineName);
	resource_.assign(pipelineName);
	
	if (vertexShader()) {
		assert(vertexShader()->type() == ShaderType::Vertex);
		glUseProgramStages(name(), GL_VERTEX_SHADER_BIT, vertexShader()->name());
	}

	if (geometryShader()) {
		assert(geometryShader()->type() == ShaderType::Geometry);
		glUseProgramStages(name(), GL_GEOMETRY_SHADER_BIT, geometryShader()->name());
	}
	
	if (fragmentShader()) {
		assert(fragmentShader()->type() == ShaderType::Fragment);
		glUseProgramStages(name(), GL_FRAGMENT_SHADER_BIT, fragmentShader()->name());
	}
}


Pipeline::~Pipeline() {
	auto pipelineName = name();
	if (pipelineName > 0) {
		glDeleteProgramPipelines(1, &pipelineName);
		resource_.clear();
	}
}


void Pipeline::bind() const {
	glBindProgramPipeline(name());
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
