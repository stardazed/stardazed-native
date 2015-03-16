// ------------------------------------------------------------------
// render::IndexBuffer - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_INDEXBUFFER_H
#define SD_RENDER_INDEXBUFFER_H

#include "system/Config.hpp"
#include "math/Vector.hpp"
#include "render/common/BufferFields.hpp"

#include <memory>
#include <limits>

namespace stardazed {
namespace render {


using TriangleI8 = math::Vector<3, uint8>;
using TriangleI16 = math::Vector<3, uint16>;
using TriangleI32 = math::Vector<3, uint32>;


// Given a highest index number to be used in an IndexBuffer, return the
// ElementType needed for each index value.
constexpr ElementType elementTypeForIndexCount(size32 maxIndex) {
	if (maxIndex <= std::numeric_limits<ElementNativeType<ElementType::UInt8>>::max())
		return ElementType::UInt8;
	if (maxIndex <= std::numeric_limits<ElementNativeType<ElementType::UInt16>>::max())
		return ElementType::UInt16;

	return ElementType::UInt32;
}


class IndexBuffer {
	ElementType indexElementType_ = ElementType::UInt8;
	size32 elementSize_ = 0, triangleSize_ = 0, count_ = 0;
	std::unique_ptr<uint8[]> storage_;

public:
	IndexBuffer()
	{}
	
	// --
	
	size32 bytesRequired(size32 triangleCount) const;
	void allocate(size32 triangleCount);
	
	// --
	
	ElementType elementType() const { return indexElementType_; }
	size32 count() const { return count_; }
};


} // ns render
} // ns stardazed

#endif
