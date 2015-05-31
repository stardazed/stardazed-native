// ------------------------------------------------------------------
// render::TriangleBuffer - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#include "render/common/TriangleBuffer.hpp"

namespace stardazed {
namespace render {


// Given a highest index number to be used in an IndexBuffer (usually the vertex
// count of a mesh), return the mininum IndexElementType needed for each index value.
constexpr IndexElementType indexTypeForVertexCount(size32 vertexCount) {
	if (vertexCount <= std::numeric_limits<uint8>::max())
		return IndexElementType::UInt8;
	if (vertexCount <= std::numeric_limits<uint16>::max())
		return IndexElementType::UInt16;
	
	return IndexElementType::UInt32;
}


void TriangleBuffer::allocateWithIndexElementType(IndexElementType et, size32 triangleCount) {
	indexElementType_ = et;
	triangleSizeBytes_ = 3 * indexElementSize(indexElementType_);
	triangleCount_ = triangleCount;

	storage_ = std::make_unique<uint8[]>(bufferSizeBytes());
}


void TriangleBuffer::allocateWithVertexCount(size32 vertexCount, size32 triangleCount) {
	allocateWithIndexElementType(indexTypeForVertexCount(vertexCount), triangleCount);
}


} // ns render
} // ns stardazed
