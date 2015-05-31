// ------------------------------------------------------------------
// render::VertexBuffer - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "render/common/VertexBuffer.hpp"
#include "math/Algorithm.hpp"

#include <algorithm>

namespace stardazed {
namespace render {


VertexBuffer::VertexBuffer(const VertexAttributeList& attrList)
: layout_(attrList)
{}


VertexBuffer::VertexBuffer(const VertexLayout& layout)
: layout_(layout)
{}


void VertexBuffer::allocate(size32 itemCount) {
	itemCount_ = itemCount;
	storage_ = std::make_unique<uint8[]>(bytesRequiredForItemCount(itemCount_));
}


void* VertexBuffer::attrBasePointer(const PositionedAttribute& attr) const {
	return storage_.get() + attr.offset;
}


void* VertexBuffer::attrBasePointer(VertexAttributeRole role) const {
	return attrBasePointer(*layout_.attrByRole(role));
}


} // ns render
} // ns stardazed
