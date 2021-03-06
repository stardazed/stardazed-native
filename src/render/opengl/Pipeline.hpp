// ------------------------------------------------------------------
// render::opengl::Pipeline - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_OPENGL_PIPELINE_H
#define SD_RENDER_OPENGL_PIPELINE_H

#include "system/Config.hpp"
#include "render/common/VertexLayout.hpp"
#include "render/common/Pipeline.hpp"
#include "render/opengl/Shader.hpp"
#include "util/ConceptTraits.hpp"

namespace stardazed {
namespace render {


class Pipeline {
	PipelineDescriptor descriptor_;
	VertexLayout vertexLayout_;
	GLResource resource_ {};
	
public:
	explicit Pipeline(const PipelineDescriptor&);
	~Pipeline();
	SD_DEFAULT_MOVE_OPS(Pipeline)
	
	GLuint name() const { return resource_.name(); }

	// FIXME: make most of these methods private and make RenderPass a friend
	void bind() const;
	void unbind() const;
	
	Shader* vertexShader() const { return descriptor_.vertexShader; }
	Shader* geometryShader() const { return descriptor_.geometryShader; }
	Shader* fragmentShader() const { return descriptor_.fragmentShader; }
	
	const VertexLayout& vertexLayout() const { return vertexLayout_; }
	
	// -- debugging
	void validateAgainstCurrentGLState() const;
};


} // ns render
} // ns stardazed

#endif
