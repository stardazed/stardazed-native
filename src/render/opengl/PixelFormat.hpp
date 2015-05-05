// ------------------------------------------------------------------
// render::opengl::PixelFormat - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_OPENGL_PIXELFORMAT_H
#define SD_RENDER_OPENGL_PIXELFORMAT_H

#include "render/common/PixelFormat.hpp"
#include "render/opengl/OpenGL.hpp"

namespace stardazed {
namespace render {


constexpr GLint glColourMaskForColourComponent(ColourComponent c) {
	switch (c) {
		case ColourComponent::Red:   return GL_RED;
		case ColourComponent::Green: return GL_GREEN;
		case ColourComponent::Blue:  return GL_BLUE;
		case ColourComponent::Alpha: return GL_ALPHA;
		case ColourComponent::Zero:  return GL_ZERO;
		case ColourComponent::One:   return GL_ONE;
	}
}


constexpr GLint glImageFormatForPixelFormat(PixelFormat format) {
	switch (format) {
		case PixelFormat::R8: return GL_RED;
		case PixelFormat::RG8: return GL_RG;
			
		case PixelFormat::RGB8: return GL_RGB;
		case PixelFormat::BGR8: return GL_BGR;
		case PixelFormat::RGBA8: return GL_RGBA;
		case PixelFormat::BGRA8: return GL_BGRA;
			
		case PixelFormat::RGB32F: return GL_RGB;
		case PixelFormat::RGBA32F: return GL_RGBA;
			
		case PixelFormat::DXT1: return GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		case PixelFormat::DXT3: return GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		case PixelFormat::DXT5: return GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
			
		case PixelFormat::Depth16I:
		case PixelFormat::Depth24I:
		case PixelFormat::Depth32I:
		case PixelFormat::Depth32F:
			return GL_DEPTH_COMPONENT;
			
		case PixelFormat::Stencil8: return GL_STENCIL_INDEX;
			
		case PixelFormat::Depth24_Stencil8:
		case PixelFormat::Depth32F_Stencil8:
			return GL_DEPTH_STENCIL;
			
		default:
			assert(!"unhandled pixel format");
			return GL_NONE;
	}
}


constexpr GLint glInternalFormatForPixelFormat(PixelFormat format) {
	switch (format) {
		case PixelFormat::R8: return GL_R8;
		case PixelFormat::RG8: return GL_RG8;
			
		case PixelFormat::RGB8: return GL_RGB8;
		case PixelFormat::BGR8: return GL_RGB8;   // swizzled
		case PixelFormat::RGBA8: return GL_RGBA8;
		case PixelFormat::BGRA8: return GL_RGBA8; // swizzled
			
		case PixelFormat::RGB32F: return GL_RGB32F;
		case PixelFormat::RGBA32F: return GL_RGBA32F;
			
		case PixelFormat::DXT1: return GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		case PixelFormat::DXT3: return GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		case PixelFormat::DXT5: return GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
			
		case PixelFormat::Depth16I: return GL_DEPTH_COMPONENT16;
		case PixelFormat::Depth24I: return GL_DEPTH_COMPONENT24;
		case PixelFormat::Depth32I: return GL_DEPTH_COMPONENT32;
		case PixelFormat::Depth32F: return GL_DEPTH_COMPONENT32F;
			
		case PixelFormat::Stencil8: return GL_STENCIL_INDEX8;
			
		case PixelFormat::Depth24_Stencil8: return GL_DEPTH24_STENCIL8;
		case PixelFormat::Depth32F_Stencil8: return GL_DEPTH32F_STENCIL8;
			
		default:
			assert(!"unhandled pixel format");
			return GL_NONE;
	}
}


constexpr GLenum glPixelDataTypeForPixelFormat(PixelFormat format) {
	assert(! pixelFormatIsCompressed(format));
	
	switch (format) {
		case PixelFormat::R8:
		case PixelFormat::RG8:
		case PixelFormat::RGB8:
		case PixelFormat::BGR8:
		case PixelFormat::RGBA8:
		case PixelFormat::BGRA8:
		case PixelFormat::Stencil8:
			return GL_UNSIGNED_BYTE;
			
		case PixelFormat::RGB32F:
		case PixelFormat::RGBA32F:
		case PixelFormat::Depth32F:
			return GL_FLOAT;
			
		case PixelFormat::Depth16I:
			return GL_UNSIGNED_SHORT;
		case PixelFormat::Depth24I:
		case PixelFormat::Depth32I:
			return GL_UNSIGNED_INT;
			
		case PixelFormat::Depth24_Stencil8:
			return GL_UNSIGNED_INT_24_8;
		case PixelFormat::Depth32F_Stencil8:
			return GL_FLOAT_32_UNSIGNED_INT_24_8_REV;
			
		default:
			assert(!"unhandled pixel format");
			return GL_NONE;
	}
}


} // ns render
} // ns stardazed

#endif
