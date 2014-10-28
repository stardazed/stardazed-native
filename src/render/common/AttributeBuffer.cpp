// ------------------------------------------------------------------
// render::AttributeBuffer - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "render/common/AttributeBuffer.hpp"

namespace stardazed {
namespace render {


size32_t calcFormatItemSize(const AttrBufferFormat& format) {
	return 16;
}


AttributeBuffer::AttributeBuffer(const AttrBufferFormat& format, size32_t itemCount)
: format_(format)
, itemSizeBytes_(calcFormatItemSize(format_))
, data_(std::make_unique<uint8_t[]>(itemSizeBytes_))
{
	
}

	
} // ns render
} // ns stardazed
