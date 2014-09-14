// ------------------------------------------------------------------
// render::OpenGLPipeline - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_OPENGLPIPELINE_H
#define SD_RENDER_OPENGLPIPELINE_H

#include "system/Config.hpp"
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


class OpenGLFaceCulling {
	bool enabled_;
	GLenum mode_;

public:
	OpenGLFaceCulling(FaceCulling);
	inline void apply() const;
};

	
class OpenGLDepthTest {
	bool enabled_;
	GLenum mode_;
	
public:
	OpenGLDepthTest(DepthTestPredicate);
	inline void apply() const;
};


} // ns detail


class OpenGLPipeline : public Pipeline {
	detail::OpenGLFaceCulling cullingMode_;
	detail::OpenGLDepthTest depthTestMode_;

	GLuint glPipeline_;
	OpenGLConstantBuffer constants_;
	std::vector<detail::ShaderConstantMapping> shaderConstants;
	
public:
	OpenGLPipeline(const PipelineDescriptor&);
	~OpenGLPipeline();
	SD_DEFAULT_MOVE_OPS(OpenGLPipeline)
	
	ConstantBuffer* constantBuffer() override { return &constants_; }
	
	void activate() override;
	void deactivate() override;
};


} // ns render
} // ns stardazed

#endif
