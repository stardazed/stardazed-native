// ------------------------------------------------------------------
// render::Shader - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_SHADER_H
#define SD_RENDER_SHADER_H

#include <memory>

namespace stardazed {
namespace render {


enum class ShaderType {
	Vertex,
	Hull,      // OpenGL term: Tesselation Control
	Domain,    // OpenGL term: Tesselation Evaluation
	Geometry,
	Fragment,  // DirectX term: pixel
	Compute
};


// forward declaration of opaque Shader class templated by shader type
template <ShaderType Type>
class Shader;


} // ns render
} // ns stardazed

#endif
