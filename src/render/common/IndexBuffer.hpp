// ------------------------------------------------------------------
// render::IndexBuffer - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_INDEXBUFFER_H
#define SD_RENDER_INDEXBUFFER_H

#include "system/Config.hpp"
#include "util/ConceptTraits.hpp"

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

	// -- read/write index values
	uint32 index(uint32 indexNr) const;
	void setIndex(uint32 indexNr, uint32 newValue);
};


} // ns render
} // ns stardazed

#endif
