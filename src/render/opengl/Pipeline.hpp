// ------------------------------------------------------------------
// render::opengl::Pipeline - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_OPENGLPIPELINE_H
#define SD_RENDER_OPENGLPIPELINE_H

#include "system/Config.hpp"
#include "render/common/Pipeline.hpp"
#include "render/opengl/ConstantBuffer.hpp"
#include "render/opengl/Shader.hpp"
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
	void apply(const ConstantBuffer&) const;
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


class Pipeline {
	detail::OpenGLFaceCulling cullingMode_;
	detail::OpenGLDepthTest depthTestMode_;

	GLuint glPipeline_;
	ConstantBuffer constants_;
	std::vector<detail::ShaderConstantMapping> shaderConstants;
	
public:
	Pipeline(const PipelineDescriptor&);
	~Pipeline();
	SD_DEFAULT_MOVE_OPS(Pipeline)
	
	ConstantBuffer* constantBuffer() { return &constants_; }
	
	void activate();
	void deactivate();
};


} // ns render
} // ns stardazed

#endif
