// ------------------------------------------------------------------
// render::IndexBuffer.cpp - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#include "render/common/IndexBuffer.hpp"
#include <limits>

namespace stardazed {
namespace render {


//  ___           _           ____         __  __
// |_ _|_ __   __| | _____  _| __ ) _   _ / _|/ _| ___ _ __
//  | || '_ \ / _` |/ _ \ \/ /  _ \| | | | |_| |_ / _ \ '__|
//  | || | | | (_| |  __/>  <| |_) | |_| |  _|  _|  __/ |
// |___|_| |_|\__,_|\___/_/\_\____/ \__,_|_| |_|  \___|_|
//

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


void IndexBuffer::indexes(uint32 baseIndexNr, uint32 outputCount, uint32* outputPtr) const {
	assert(baseIndexNr < indexCount());
	assert(baseIndexNr + outputCount < indexCount());
	
	uint8* basePtr = static_cast<uint8*>(basePointer());
	uint32 offsetBytes = indexElementSize() * baseIndexNr;
	basePtr += offsetBytes;

	if (indexElementType() == IndexElementType::UInt32) {
		auto sizedBasePtr = reinterpret_cast<uint32*>(basePtr);
		for (size32 ix = 0; ix < outputCount; ++ix) {
			*outputPtr++ = *sizedBasePtr++;
		}
	}
	else if (indexElementType() == IndexElementType::UInt16) {
		auto sizedBasePtr = reinterpret_cast<uint16*>(basePtr);
		for (size32 ix = 0; ix < outputCount; ++ix) {
			*outputPtr++ = *sizedBasePtr++;
		}
	}
	else {
		for (size32 ix = 0; ix < outputCount; ++ix) {
			*outputPtr++ = *basePtr++;
		}
	}
}


void IndexBuffer::setIndexes(uint32 baseIndexNr, size32 sourceCount, const uint32* sourcePtr) {
	assert(baseIndexNr < indexCount());
	assert(baseIndexNr + sourceCount < indexCount());
	
	uint8* base = static_cast<uint8*>(basePointer());
	uint32 offsetBytes = indexElementSize() * baseIndexNr;
	base += offsetBytes;
	
	if (indexElementType() == IndexElementType::UInt32) {
		auto sizedBasePtr = reinterpret_cast<uint32*>(base);
		for (size32 ix = 0; ix < sourceCount; ++ix) {
			*sizedBasePtr++ = *sourcePtr++;
		}
	}
	else if (indexElementType() == IndexElementType::UInt16) {
		auto sizedBasePtr = reinterpret_cast<uint16*>(base);
		for (size32 ix = 0; ix < sourceCount; ++ix) {
			*sizedBasePtr++ = static_cast<uint16>(*sourcePtr++);
		}
	}
	else {
		for (size32 ix = 0; ix < sourceCount; ++ix) {
			*base++ = static_cast<uint8>(*sourcePtr++);
		}
	}
}


//  ___           _           ____         __  __          _____     _                   _    __     ___
// |_ _|_ __   __| | _____  _| __ ) _   _ / _|/ _| ___ _ _|_   _| __(_) __ _ _ __   __ _| | __\ \   / (_) _____      __
//  | || '_ \ / _` |/ _ \ \/ /  _ \| | | | |_| |_ / _ \ '__|| || '__| |/ _` | '_ \ / _` | |/ _ \ \ / /| |/ _ \ \ /\ / /
//  | || | | | (_| |  __/>  <| |_) | |_| |  _|  _|  __/ |   | || |  | | (_| | | | | (_| | |  __/\ V / | |  __/\ V  V /
// |___|_| |_|\__,_|\___/_/\_\____/ \__,_|_| |_|  \___|_|   |_||_|  |_|\__,_|_| |_|\__, |_|\___| \_/  |_|\___| \_/\_/
//                                                                                 |___/

IndexBufferTriangleView::IndexBufferTriangleView(IndexBuffer& indexBuffer)
: indexBuffer_(indexBuffer)
{
	assert(indexBuffer_.primitiveType() == PrimitiveType::Triangle);
}


void IndexBufferTriangleView::setTriangle(uint32 triangleIndex, const Triangle& tri) {
	indexBuffer_.setIndexes(triangleIndex * 3, 3, tri.data);
}


Triangle IndexBufferTriangleView::triangleAtIndex(uint32 triangleIndex) const {
	Triangle result;
	indexBuffer_.indexes(triangleIndex * 3, 3, result.data);
	return result;
}


} // ns render
} // ns stardazed
