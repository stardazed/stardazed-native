// ------------------------------------------------------------------
// render::IndexBuffer - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_INDEXBUFFER_H
#define SD_RENDER_INDEXBUFFER_H

#include "system/Config.hpp"
#include "util/ConceptTraits.hpp"
#include "math/Vector.hpp"

#include <memory>
#include <limits>

namespace stardazed {
namespace render {


enum class IndexElementType {
	UInt8,
	UInt16,
	UInt32
};


enum class PrimitiveType {
	Point,
	Line,
	LineStrip,
	Triangle,
	TriangleStrip
};


constexpr size32 indexElementTypeSizeBytes(IndexElementType iet) {
	switch (iet) {
		case IndexElementType::UInt8: return sizeof(uint8);
		case IndexElementType::UInt16: return sizeof(uint16);
		case IndexElementType::UInt32: return sizeof(uint32);
	}
}


constexpr IndexElementType minimumIndexElementTypeForVertexCount(uint32 vertexCount) {
	// FIXME: add platform switch here, Metal does not support UInt8 indices
	if (vertexCount <= std::numeric_limits<uint8>::max())
		return IndexElementType::UInt8;
	if (vertexCount <= std::numeric_limits<uint16>::max())
		return IndexElementType::UInt16;
	
	return IndexElementType::UInt32;
}


class IndexBuffer {
	PrimitiveType primitiveType_ = PrimitiveType::Point;
	IndexElementType indexElementType_ = IndexElementType::UInt8;
	uint32 indexCount_ = 0, primitiveCount_ = 0, indexElementSize_ = 0;
	std::unique_ptr<uint8[]> storage_;

public:
	void allocate(PrimitiveType, IndexElementType, uint32 primitiveCount);

	// -- observers
	PrimitiveType primitiveType() const { return primitiveType_; }
	IndexElementType indexElementType() const { return indexElementType_; }

	uint32 primitiveCount() const { return primitiveCount_; }
	uint32 indexCount() const { return indexCount_; }
	uint32 indexElementSize() const { return indexElementSize_; }

	size32 bufferSizeBytes() const { return indexCount() * indexElementSize(); }
	void* basePointer() const { return storage_.get(); }

	// -- read/write indexes
	void indexes(uint32 baseIndexNr, uint32 outputCount, uint32* outputPtr) const;
	uint32 index(uint32 indexNr) const {
		uint32 result = 0;
		indexes(indexNr, 1, &result);
		return result;
	}

	void setIndexes(uint32 baseIndexNr, size32 sourceCount, const uint32* sourcePtr);
	void setIndex(uint32 indexNr, uint32 newValue) {
		setIndexes(indexNr, 1, &newValue);
	}
};


using Triangle = math::Vector<3, uint32>;


class IndexBufferTriangleView {
	IndexBuffer& indexBuffer_;

public:
	IndexBufferTriangleView(IndexBuffer&);
	
	uint32 triangleCount() const {
		return indexBuffer_.primitiveCount();
	}
	
	void setTriangle(uint32 triangleIndex, const Triangle& tri);
	void setTriangle(uint32 triangleIndex, uint32 a, uint32 b, uint32 c) {
		setTriangle(triangleIndex, { a, b, c });
	}

	Triangle triangleAtIndex(uint32) const;
};


} // ns render
} // ns stardazed

#endif
