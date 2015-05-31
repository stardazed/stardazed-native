// ------------------------------------------------------------------
// render::opengl::RenderPass - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_OPENGL_RENDERPASS_H
#define SD_RENDER_OPENGL_RENDERPASS_H

#include "render/common/RenderPass.hpp"
#include "render/opengl/Texture.hpp"

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


class RenderPass {
public:
	explicit RenderPass(const RenderPassDescriptor&);
	~RenderPass();

	
};


} // ns render
} // ns stardazed

#endif
