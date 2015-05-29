// ------------------------------------------------------------------
// render::PixelFormat - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_PIXELFORMAT_H
#define SD_RENDER_PIXELFORMAT_H

#include "system/Config.hpp"

namespace stardazed {
namespace render {


enum class ColourComponent {
	Red,
	Green,
	Blue,
	Alpha,
	Zero,
	One
};


enum class PixelFormat : uint32 {
	None,
	
	// 8-bit component
	R8,
	RG8,
	
	RGB8,
	BGR8,
	RGBA8,
	BGRA8,
	
	// 32-bit component
	RGB32F,
	RGBA32F,
	
	// S3TC
	DXT1,
	DXT3,
	DXT5,
	
	// Depth / Stencil
	Depth16I,
	Depth24I,
	Depth32I,
	Depth32F,
	DepthShadow = Depth32F,
	
	Stencil8,
	
	Depth24_Stencil8,
	Depth32F_Stencil8,
	DepthShadow_Stencil8 = Depth32F_Stencil8
};


constexpr bool pixelFormatIsCompressed(PixelFormat format) {
	return format == PixelFormat::DXT1 ||
		   format == PixelFormat::DXT3 ||
		   format == PixelFormat::DXT5;
}


constexpr bool pixelFormatIsDepthFormat(PixelFormat format) {
	return format == PixelFormat::Depth16I ||
		   format == PixelFormat::Depth24I ||
		   format == PixelFormat::Depth32I ||
		   format == PixelFormat::Depth32F;
}


constexpr bool pixelFormatIsStencilFormat(PixelFormat format) {
	return format == PixelFormat::Stencil8;
}


constexpr bool pixelFormatIsDepthStencilFormat(PixelFormat format) {
	return format == PixelFormat::Depth24_Stencil8 ||
		   format == PixelFormat::Depth32F_Stencil8;
}


constexpr size32 pixelFormatBytesPerElement(PixelFormat format) {
	// Element means a pixel for non-compressed formats
	// and a block for compressed formats
	
	switch (format) {
		case PixelFormat::R8:
		case PixelFormat::Stencil8:
			return 1;

		case PixelFormat::RG8:
		case PixelFormat::Depth16I:
			return 2;

		case PixelFormat::RGB8:
		case PixelFormat::BGR8:
			return 3;

		case PixelFormat::RGBA8:
		case PixelFormat::BGRA8:
		case PixelFormat::Depth24I:
		case PixelFormat::Depth32I:
		case PixelFormat::Depth32F:
		case PixelFormat::Depth24_Stencil8:
			return 4;

		case PixelFormat::RGB32F:
			return 12;
		
		case PixelFormat::RGBA32F:
		case PixelFormat::Depth32F_Stencil8:
			return 16;

		// -- compressed formats

		case PixelFormat::DXT1:
			return 8;
		
		case PixelFormat::DXT3:
		case PixelFormat::DXT5:
			return 16;

		default:
			assert(!"unhandled pixel buffer format");
			return 0;
	}
}


} // ns render
} // ns stardazed

#endif
