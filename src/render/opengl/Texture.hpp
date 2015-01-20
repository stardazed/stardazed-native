// ------------------------------------------------------------------
// render::opengl::Texture - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_OPENGLTEXTURE_H
#define SD_RENDER_OPENGLTEXTURE_H

#include "system/Config.hpp"
#include "render/opengl/OpenGL.hpp"

#include <string>

namespace stardazed {
namespace render {


enum class TextureKind {
	Single2D,
	CubeMap
};


enum class TextureDataFormat {
	RGBA8,
	
	DXT1,
	DXT3,
	DXT5
};


class TextureDataProvider {
public:
	virtual ~TextureDataProvider() {}
	
	virtual size32 width() = 0;
};


class Texture {
	TextureKind kind_;
	GLenum glTexTarget_;
	GLuint glTex_;
	
public:
	Texture(TextureKind kind);
	~Texture();
	
	TextureKind kind() const { return kind_; }
	
	void allocate(size32 width, size32 height, uint8_t levels, TextureDataFormat format);
	
	// -- image files
	void loadDDS(const std::string& resourcePath);
};


} // ns render
} // ns stardazed

#endif
