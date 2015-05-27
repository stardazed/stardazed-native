// ------------------------------------------------------------------
// render::Texture - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_TEXTURE_H
#define SD_RENDER_TEXTURE_H

#include "system/Config.hpp"
#include "math/Algorithm.hpp"
#include "render/common/PixelFormat.hpp"
#include "render/common/PixelBuffer.hpp"

namespace stardazed {
namespace render {


enum class TextureClass : uint32 {
	Tex1D,
	Tex2D,
	Tex3D,
	TexCube
};


enum class TextureUsageHint : uint32 {
	Normal,
	RenderTargetOnly
};


enum class UseMipMaps : bool32 {
	No = 0,
	Yes = 1
};


// the numerical constant for each face is also the layer index in the cubemap
enum class CubeMapFace : uint32 {
	PosX,
	NegX,
	PosY,
	NegY,
	PosZ,
	NegZ
};


struct MipMapRange {
	// default values guarantee full mip range for any imaginable texture
	uint32 baseLevel = 0;
	uint32 numLevels = 128;
};


struct TextureDescriptor {
	TextureClass textureClass = TextureClass::Tex2D;
	PixelFormat pixelFormat = PixelFormat::None;
	TextureUsageHint usageHint = TextureUsageHint::Normal;
	PixelDimensions dim;
	size32 layers = 1, mipmaps = 1, samples = 1;
};


inline size32 maxMipLevelsForDimension(size32 dim) {
	return 1 + (size32)std::floor(std::log2((float)dim));
}


// -- helpers functions to quickly setup common texture descriptors

TextureDescriptor makeTexDesc1D(PixelFormat pixelFormat, size32 width, UseMipMaps mipmapped);
TextureDescriptor makeTexDesc1DArray(PixelFormat pixelFormat, size32 width, size32 layers, UseMipMaps mipmapped);

TextureDescriptor makeTexDesc2D(PixelFormat pixelFormat, size32 width, size32 height, UseMipMaps mipmapped);
TextureDescriptor makeTexDesc2DArray(PixelFormat pixelFormat, size32 width, size32 height, size32 layers, UseMipMaps mipmapped);
TextureDescriptor makeTexDesc2DMultisample(PixelFormat pixelFormat, size32 width, size32 height, size32 samples);

TextureDescriptor makeTexDesc3D(PixelFormat pixelFormat, size32 width, size32 height, size32 depth, UseMipMaps mipmapped);

TextureDescriptor makeTexDescCube(PixelFormat pixelFormat, size32 dimension, UseMipMaps mipmapped);

TextureDescriptor makeTexDescFromPixelBuffer(const PixelBuffer&, TextureClass textureClass);
TextureDescriptor makeTexDescFromPixelDataProvider(const PixelDataProvider& provider, TextureClass textureClass);


} // ns render
} // ns stardazed

#endif
