// ------------------------------------------------------------------
// render::OpenGLPipeline.cpp - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "render/OpenGLPipeline.hpp"
#include "render/OpenGLShader.hpp"

namespace stardazed {
namespace render {


OpenGLPipeline::OpenGLPipeline() {
	glGenProgramPipelines(1, &pipelineName);
}


OpenGLPipeline::~OpenGLPipeline() {
	glDeleteProgramPipelines(1, &pipelineName);
}


void OpenGLPipeline::setVertexStage(const VertexShaderRef& vs) {
	auto glvs = std::static_pointer_cast<const OpenGLShaderRef<ShaderType::Vertex>&>(vs);
	glUseProgramStages(pipelineName, GL_VERTEX_SHADER_BIT, glvs.glName);
}


void OpenGLPipeline::setTesselationControlStage(const TesselationControlShaderRef& tcs) {
	auto gltcs = static_cast<const OpenGLShader<ShaderType::TesselationControl>&>(tcs);
	glUseProgramStages(pipelineName, GL_TESS_CONTROL_SHADER_BIT, gltcs.glName);
}


void OpenGLPipeline::setTesselationEvalStage(const TesselationEvalShaderRef& tes) {
	auto gltes = static_cast<const OpenGLShader<ShaderType::TesselationEval>&>(tes);
	glUseProgramStages(pipelineName, GL_TESS_EVALUATION_SHADER_BIT, gltes.glName);
}


void OpenGLPipeline::setGeometryStage(const GeometryShaderRef& gs) {
	auto glgs = static_cast<const OpenGLShader<ShaderType::Geometry>&>(gs);
	glUseProgramStages(pipelineName, GL_GEOMETRY_SHADER_BIT, glgs.glName);
}


void OpenGLPipeline::setFragmentStage(const FragmentShaderRef& fs) {
	auto glfs = static_cast<const OpenGLShader<ShaderType::Fragment>&>(fs);
	glUseProgramStages(pipelineName, GL_FRAGMENT_SHADER_BIT, glfs.glName);
}


void OpenGLPipeline::activate() {
	glBindProgramPipeline(pipelineName);
}


void OpenGLPipeline::deactivate() {
	glBindProgramPipeline(0);
}


} // ns render
} // ns stardazed
