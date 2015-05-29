// ------------------------------------------------------------------
// render::opengl::Pipeline - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_OPENGLPIPELINE_H
#define SD_RENDER_OPENGLPIPELINE_H

#include "system/Config.hpp"
#include "render/common/Pipeline.hpp"
#include "render/opengl/Shader.hpp"
#include "util/ConceptTraits.hpp"

#include <vector>


namespace stardazed {
namespace render {


namespace detail {

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



struct PipelineDescriptor {
	FaceCulling faceCulling = FaceCulling::Disabled;
	DepthTestPredicate depthTest = DepthTestPredicate::Disabled;
	
	Shader* vertexShader = nullptr;
	Shader* geometryShader = nullptr;
	Shader* fragmentShader = nullptr;
};


struct SSOPipelineDescriptor {
	FaceCulling faceCulling = FaceCulling::Disabled;
	DepthTestPredicate depthTest = DepthTestPredicate::Disabled;
	
	Program* vertexProgram = nullptr;
	Program* geometryProgram = nullptr;
	Program* fragmentProgram = nullptr;
};



class Pipeline {
	detail::OpenGLFaceCulling cullingMode_;
	detail::OpenGLDepthTest depthTestMode_;
	
	Program* vertexProgram_ = nullptr;
	Program* geometryProgram_ = nullptr;
	Program* fragmentProgram_ = nullptr;

	bool32 usesPipeline = false;
	union {
		GLuint glPipeline_;
		Program pipeProgram_;
	};
	
public:
	explicit Pipeline(const PipelineDescriptor&);
	explicit Pipeline(const SSOPipelineDescriptor&);
	~Pipeline();
	SD_DEFAULT_MOVE_OPS(Pipeline)
	
	void bind();
	
	Program* vertexProgram() const { return vertexProgram_; }
	Program* geometryProgram() const { return geometryProgram_; }
	Program* fragmentProgram() const { return fragmentProgram_; }
};


} // ns render
} // ns stardazed

#endif
