// ------------------------------------------------------------------
// render::IndexBuffer - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "render/common/IndexBuffer.hpp"

namespace stardazed {
namespace render {


// Given a highest index number to be used in an IndexBuffer (usually the vertex
// count of a mesh), return the ElementType needed for each index value.
constexpr ElementType indexTypeForVertexCount(size32 vertexCount) {
	if (vertexCount <= std::numeric_limits<ElementNativeType<ElementType::UInt8>>::max())
		return ElementType::UInt8;
	if (vertexCount <= std::numeric_limits<ElementNativeType<ElementType::UInt16>>::max())
		return ElementType::UInt16;
	
	return ElementType::UInt32;
}


void TriangleBuffer::allocateWithElementType(ElementType et, size32 triangleCount) {
	indexElementType_ = et;
	triangleSizeBytes_ = 3 * elementSize(indexElementType_);
	triangleCount_ = triangleCount;

	storage_ = std::make_unique<uint8[]>(bufferSizeBytes());
}


void TriangleBuffer::allocateWithVertexCount(size32 vertexCount, size32 triangleCount) {
	allocateWithElementType(indexTypeForVertexCount(vertexCount), triangleCount);
}


} // ns render
} // ns stardazed
