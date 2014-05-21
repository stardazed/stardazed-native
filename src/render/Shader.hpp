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


class Shader {
public:
	virtual ~Shader() = default;

	virtual ShaderType type() const = 0;
};


} // ns render
} // ns stardazed

#endif
