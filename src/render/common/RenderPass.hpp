// ------------------------------------------------------------------
// render::RenderPass - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_RENDERPASS_H
#define SD_RENDER_RENDERPASS_H

#include "system/Config.hpp"

#include <array>

namespace stardazed {
namespace render {


class Texture;
class Shader;
class Sampler;


struct TextureBinding {
	Texture* tex = nullptr;
	Sampler* sampler = nullptr;
};


struct RenderPass {
	std::array<TextureBinding, 16> textures;
};


struct Pipeline {
	Shader* vertexShader = nullptr;
	Shader* geometryShader = nullptr;
	Shader* fragmentShader = nullptr;

	PositionedAttributeList vertex
};


} // ns render
} // ns stardazed

#endif
