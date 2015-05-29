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


class RenderPass {
public:
	explicit RenderPass(const RenderPassDescriptor&);
	~RenderPass();

	
};


} // ns render
} // ns stardazed

#endif
