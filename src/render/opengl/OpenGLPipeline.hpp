// ------------------------------------------------------------------
// render::OpenGLPipeline - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_OPENGLPIPELINE_H
#define SD_RENDER_OPENGLPIPELINE_H

#include "render/Pipeline.hpp"
#include "render/opengl/OpenGLConstantBuffer.hpp"
#include "render/opengl/OpenGLShader.hpp"
#include "util/ConceptTraits.hpp"

#include <vector>


namespace stardazed {
namespace render {


namespace detail {

class ShaderConstantMapping {
	GLuint program_;
	GLint mvMat_, mvpMat_, normalMat_;

public:
	explicit ShaderConstantMapping(GLuint shaderProgram);
	void apply(const OpenGLConstantBuffer&) const;
};
	
} // ns detail


class OpenGLPipeline : public Pipeline<OpenGLShader> {
	GLuint glPipeline_;
	OpenGLConstantBuffer constants_;
	std::vector<detail::ShaderConstantMapping> shaderConstants;
	
public:
	OpenGLPipeline();
	~OpenGLPipeline();
	SD_DEFAULT_MOVE_OPS(OpenGLPipeline)
	
	ConstantBuffer* constantBuffer() override { return &constants_; }
	
	void attachShader(const OpenGLShader&) override;
	
	void activate() override;
	void deactivate() override;
};


} // ns render
} // ns stardazed

#endif
