// ------------------------------------------------------------------
// render::Texture - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_TEXTURE_H
#define SD_RENDER_TEXTURE_H

#include "system/Config.hpp"
#include "render/common/PixelFormat.hpp"
#include "render/common/PixelBuffer.hpp"

namespace stardazed {
namespace render {


enum class CubeMapFace : size32 {
	PosX,
	NegX,
	PosY,
	NegY,
	PosZ,
	NegZ
};


enum class TextureClass : uint32 {
	Tex1D,
	Tex2D,
	Tex3D,
	TexCube
};


struct TextureDescriptor {
	TextureClass textureClass;
	PixelFormat pixelFormat;
	size32 width, height, depth, layers, mipmaps, samples;
};


} // ns render
} // ns stardazed

#endif
