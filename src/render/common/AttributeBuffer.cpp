// ------------------------------------------------------------------
// render::AttributeBuffer - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "render/common/AttributeBuffer.hpp"

namespace stardazed {
namespace render {


AttributeBuffer::AttributeBuffer(const AttributeList& attrList, size32_t itemCount) {
	attrList_.reserve(attrList.size());
	size32_t offset = 0;

	for (auto& attr : attrList) {
		offset = nextOffsetForFormatAndAlignment(offset, attr.format, alignment);
		attrList_.emplace_back(attr, offset);
	}
}

	
} // ns render
} // ns stardazed
