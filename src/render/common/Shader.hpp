// ------------------------------------------------------------------
// render::Shader - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_SHADER_H
#define SD_RENDER_SHADER_H

#include "system/Config.hpp"

namespace stardazed {
namespace render {


enum class ShaderType {
	None,
	Vertex,
	Hull,      // OpenGL term: Tesselation Control
	Domain,    // OpenGL term: Tesselation Evaluation
	Geometry,
	Fragment,  // DirectX term: pixel
	Compute
};


} // ns render
} // ns stardazed

#endif
