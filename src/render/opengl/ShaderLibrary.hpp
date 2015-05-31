// ------------------------------------------------------------------
// render::opengl::ShaderLibrary - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_OPENGL_SHADERLIBRARY_H
#define SD_RENDER_OPENGL_SHADERLIBRARY_H

#include "system/Config.hpp"
#include "util/ConceptTraits.hpp"
#include "filesystem/FileSystem.hpp"
#include "render/common/PixelFormat.hpp"
#include "render/common/PixelBuffer.hpp"
#include "render/common/FrameBuffer.hpp"
#include "render/common/Shader.hpp"
#include "render/opengl/OpenGL.hpp"

namespace stardazed {
namespace render {


namespace detail {
	class GLProgram {
		GLuint glProgram_;
	public:
		
	};
}


class Shader {
	GLuint glShader_;
	ShaderType type_;
public:
};


class ShaderLibrary {
public:
	explicit ShaderLibrary(const fs::Path& basePath);
	
	Shader* getShader(ShaderType, const std::string& name);
};


} // ns render
} // ns stardazed

#endif
