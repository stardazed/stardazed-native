// ------------------------------------------------------------------
// render::opengl::Light - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_OPENGL_LIGHT_H
#define SD_RENDER_OPENGL_LIGHT_H

#include "system/Config.hpp"
#include "render/common/Light.hpp"
#include "render/opengl/Buffer.hpp"

namespace stardazed {
namespace render {


class Light {
	Buffer shaderLights_;
	uint32 maxShaderLights_, shaderBlockSizeBytes_;

public:
	Light();
};


} // ns render
} // ns stardazed

#endif
