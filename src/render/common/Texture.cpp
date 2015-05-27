// ------------------------------------------------------------------
// render::Texture.cpp - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "render/common/Texture.hpp"

namespace stardazed {
namespace render {


TextureDescriptor makeTexDesc1D(PixelFormat pixelFormat, size32 width, UseMipMaps mipmapped) {
	TextureDescriptor td {};
	td.textureClass = TextureClass::Tex1D;
	td.pixelFormat  = pixelFormat;
	td.dim.width = width;
	if (mipmapped == UseMipMaps::Yes)
		td.mipmaps = maxMipLevelsForDimension(width);

	return td;
}


TextureDescriptor makeTexDesc1DArray(PixelFormat pixelFormat, size32 width, size32 layers, UseMipMaps mipmapped) {
	auto td = makeTexDesc1D(pixelFormat, width, mipmapped);
	td.layers = layers;
	
	return td;
}


TextureDescriptor makeTexDesc2D(PixelFormat pixelFormat, size32 width, size32 height, UseMipMaps mipmapped) {
	TextureDescriptor td {};
	td.textureClass = TextureClass::Tex2D;
	td.pixelFormat  = pixelFormat;
	td.dim.width = width;
	td.dim.height = height;
	if (mipmapped == UseMipMaps::Yes)
		td.mipmaps = maxMipLevelsForDimension(math::max(width, height));

	return td;
}


TextureDescriptor makeTexDesc2DArray(PixelFormat pixelFormat, size32 width, size32 height, size32 layers, UseMipMaps mipmapped) {
	auto td = makeTexDesc2D(pixelFormat, width, height, mipmapped);
	td.layers = layers;
	
	return td;
}


TextureDescriptor makeTexDesc2DMultisample(PixelFormat pixelFormat, size32 width, size32 height, size32 samples) {
	auto td = makeTexDesc2D(pixelFormat, width, height, UseMipMaps::No);
	td.samples = samples;
	
	return td;
}


TextureDescriptor makeTexDesc3D(PixelFormat pixelFormat, size32 width, size32 height, size32 depth, UseMipMaps mipmapped) {
	TextureDescriptor td {};
	td.textureClass = TextureClass::Tex3D;
	td.pixelFormat  = pixelFormat;
	td.dim.width = width;
	td.dim.height = height;
	td.dim.depth = depth;
	if (mipmapped == UseMipMaps::Yes)
		td.mipmaps = maxMipLevelsForDimension(math::max(math::max(width, height), depth));

	return td;
}


TextureDescriptor makeTexDescCube(PixelFormat pixelFormat, size32 dimension, UseMipMaps mipmapped) {
	TextureDescriptor td {};
	td.textureClass = TextureClass::TexCube;
	td.pixelFormat  = pixelFormat;
	td.dim.width = dimension;
	td.dim.height = dimension;
	if (mipmapped == UseMipMaps::Yes)
		td.mipmaps = maxMipLevelsForDimension(dimension);

	return td;
}


TextureDescriptor makeTexDescFromPixelBuffer(const PixelBuffer& pixelBuffer, TextureClass textureClass) {
	TextureDescriptor td {};
	td.textureClass = textureClass;
	td.pixelFormat = pixelBuffer.format;
	td.dim = pixelBuffer.dim;
	
	switch (textureClass) {
		case TextureClass::Tex1D:
			assert(td.dim.depth == 1);

			// 2D pixelbuffer -> 1D array
			if (td.dim.height > 1) {
				td.layers = td.dim.height;
				td.dim.height = 1;
			}
			break;
		
		case TextureClass::Tex2D:
			// 3D pixelbuffer -> 2D array
			if (td.dim.depth > 1) {
				td.layers = td.dim.depth;
				td.dim.depth = 1;
			}
			break;
		
		case TextureClass::Tex3D:
			// no special dimension handling
			break;
		
		case TextureClass::TexCube:
			assert(td.dim.width == td.dim.height);
			assert(td.dim.depth == 1);
			break;
	}
	
	return td;
}


TextureDescriptor makeTexDescFromPixelDataProvider(const PixelDataProvider& provider, TextureClass textureClass) {
	auto pixelBuffer = provider.pixelBufferForLevel(0);
	auto td = makeTexDescFromPixelBuffer(pixelBuffer, textureClass);
	td.mipmaps = provider.mipMapCount();
	return td;
}


} // ns render
} // ns stardazed
