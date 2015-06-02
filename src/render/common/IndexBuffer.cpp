// ------------------------------------------------------------------
// render::IndexBuffer.cpp - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#include "render/common/IndexBuffer.hpp"
#include <limits>

namespace stardazed {
namespace render {


void IndexBuffer::allocate(PrimitiveType primitiveType, IndexElementType elementType, uint32 primitiveCount) {
	primitiveType_ = primitiveType;
	indexElementType_ = elementType;
	indexElementSize_ = indexElementTypeSizeBytes(indexElementType_);
	primitiveCount_ = primitiveCount;

	switch (primitiveType) {
		case PrimitiveType::Point:
			indexCount_ = primitiveCount;
			break;
		case PrimitiveType::Line:
			indexCount_ = primitiveCount * 2;
			break;
		case PrimitiveType::LineStrip:
			indexCount_ = primitiveCount + 1;
			break;
		case PrimitiveType::Triangle:
			indexCount_ = primitiveCount * 3;
			break;
		case PrimitiveType::TriangleStrip:
			indexCount_ = primitiveCount + 2;
			break;
	}

	storage_ = std::make_unique<uint8[]>(bufferSizeBytes());
}


uint32 IndexBuffer::index(uint32 indexNr) const {
	assert(indexNr < indexCount());

	uint8* base = static_cast<uint8*>(basePointer());
	uint32 offsetBytes = indexElementSize() * indexNr;
	base += offsetBytes;

	if (indexElementType() == IndexElementType::UInt32)
		return *(reinterpret_cast<uint32*>(base));
	else if (indexElementType() == IndexElementType::UInt16)
		return *(reinterpret_cast<uint16*>(base));

	return *(base);
}


void IndexBuffer::setIndex(uint32 indexNr, uint32 newValue) {
	assert(indexNr < indexCount());

	uint8* base = static_cast<uint8*>(basePointer());
	uint32 offsetBytes = indexElementSize() * indexNr;
	base += offsetBytes;
	
	if (indexElementType() == IndexElementType::UInt32)
		*(reinterpret_cast<uint32*>(base)) = newValue;
	else if (indexElementType() == IndexElementType::UInt16)
		*(reinterpret_cast<uint16*>(base)) = static_cast<uint16>(newValue);
	else
		*(base) = static_cast<uint8>(newValue);
}


} // ns render
} // ns stardazed
