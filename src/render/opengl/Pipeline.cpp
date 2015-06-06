// ------------------------------------------------------------------
// render::opengl::Pipeline.cpp - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "render/opengl/Pipeline.hpp"

namespace stardazed {
namespace render {


constexpr GLenum glBlendEqForBlendOperation(BlendOperation op) {
	switch (op) {
		case BlendOperation::Add: return GL_FUNC_ADD;
		case BlendOperation::Subtract: return GL_FUNC_SUBTRACT;
		case BlendOperation::ReverseSubtract: GL_FUNC_REVERSE_SUBTRACT;
		case BlendOperation::Min: return GL_MIN;
		case BlendOperation::Max: return GL_MAX;
	}
}


constexpr GLenum glBlendFuncForBlendFactor(BlendFactor factor) {
	switch (factor) {
		case BlendFactor::Zero: return GL_ZERO;
		case BlendFactor::One: return GL_ONE;
		case BlendFactor::SourceColour: return GL_SRC_COLOR;
		case BlendFactor::OneMinusSourceColour: GL_ONE_MINUS_SRC_COLOR;
		case BlendFactor::DestColour: return GL_DST_COLOR;
		case BlendFactor::OneMinusDestColour: return GL_ONE_MINUS_DST_COLOR;
		case BlendFactor::SourceAlpha: return GL_SRC_ALPHA;
		case BlendFactor::OneMinusSourceAlpha: return GL_ONE_MINUS_SRC_ALPHA;
		case BlendFactor::SourceAlphaSaturated: return GL_SRC_ALPHA_SATURATE;
		case BlendFactor::DestAlpha: return GL_DST_ALPHA;
		case BlendFactor::OneMinusDestAlpha: return GL_ONE_MINUS_DST_ALPHA;
		case BlendFactor::ConstantColour: return GL_CONSTANT_COLOR;
		case BlendFactor::OneMinusConstantColour: return GL_ONE_MINUS_CONSTANT_COLOR;
		case BlendFactor::ConstantAlpha: return GL_CONSTANT_ALPHA;
		case BlendFactor::OneMinusConstantAlpha: return GL_ONE_MINUS_CONSTANT_ALPHA;
	}
}


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

	if (! descriptor_.rasterizerEnabled) {
		glEnable(GL_RASTERIZER_DISCARD);
	}
	else {
		// -- apply write masks and blending modes for enabled colour attachments
		uint32 attIndex = 0;
		bool useBlending = false;

		for (auto& caDesc : descriptor_.colourAttachments) {
			if (caDesc.pixelFormat != PixelFormat::None) {
				glColorMaski(attIndex, caDesc.writeMask.red, caDesc.writeMask.green, caDesc.writeMask.blue, caDesc.writeMask.alpha);

				if (caDesc.blending.enabled) {
					useBlending = true;
					auto rgbEq = glBlendEqForBlendOperation(caDesc.blending.rgbBlendOp);
					auto alphaEq = glBlendEqForBlendOperation(caDesc.blending.alphaBlendOp);
					glBlendEquationSeparatei(attIndex, rgbEq, alphaEq);

					auto rgbSrcFn = glBlendFuncForBlendFactor(caDesc.blending.sourceRGBFactor);
					auto alphaSrcFn = glBlendFuncForBlendFactor(caDesc.blending.sourceAlphaFactor);
					auto rgbDestFn = glBlendFuncForBlendFactor(caDesc.blending.destRGBFactor);
					auto alphaDestFn = glBlendFuncForBlendFactor(caDesc.blending.destAlphaFactor);
					glBlendFuncSeparatei(attIndex, rgbSrcFn, rgbDestFn, alphaSrcFn, alphaDestFn);
				}
			}
			
			++attIndex;
		}

		// -- in mother GL, global state blends YOU!
		if (useBlending)
			glEnable(GL_BLEND);
	}
}


void Pipeline::unbind() const {
	glBindProgramPipeline(0);

	// FIXME: since these are only set in Pipeline, can we move
	// leave these out and just set/unset all the values in bind()?

	if (! descriptor_.rasterizerEnabled) {
		glDisable(GL_RASTERIZER_DISCARD);
	}
	else {
		// -- restore all write masks and blending modes to defaults
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glDisable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ZERO);
	}
}


} // ns render
} // ns stardazed
