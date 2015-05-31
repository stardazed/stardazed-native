// ------------------------------------------------------------------
// render::VertexLayout.cpp - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#include "render/common/VertexLayout.hpp"
#include "math/Algorithm.hpp"

#include <algorithm>

namespace stardazed {
namespace render {


constexpr size32 alignFieldOnSize(size32 size, size32 offset) {
	// FIXME: this will fail if size is not a power of 2
	// extend to nearest power of 2, then - 1
	size32 mask = size - 1;
	return (offset + mask) & ~mask;
}


constexpr size32 alignVertexField(VertexField field, size32 offset) {
	return alignFieldOnSize(vertexFieldElementSizeBytes(field), offset);
}


VertexLayout::VertexLayout(const VertexAttributeList& attrList)
: attributeCount_(size32_cast(attrList.size()))
{
	assert(attributeCount() <= maxVertexAttributes());

	size32 offset = 0, maxElemSize = 0;

	// calculate positioning of successive attributes in linear item
	std::transform(begin(attrList), end(attrList), begin(attrs_),
		[&offset, &maxElemSize](const VertexAttribute& attr) -> PositionedAttribute {
			auto size = vertexFieldSizeBytes(attr.field);
			maxElemSize = math::max(maxElemSize, vertexFieldElementSizeBytes(attr.field));

			auto alignedOffset = alignVertexField(attr.field, offset);
			offset = alignedOffset + size;
			return { attr, alignedOffset };
		});

	// align full item size on boundary of biggest element in attribute list, with min of float boundary
	maxElemSize = math::max(sizeof32<float>(), maxElemSize);
	vertexSizeBytes_ = alignFieldOnSize(maxElemSize, offset);
}


const PositionedAttribute* VertexLayout::attrByPredicate(std::function<bool(const PositionedAttribute&)> pred) const {
	auto from = begin(attrs_),
		 upto = from + attributeCount();
	auto it = std::find_if(from, upto, pred);

	if (it == upto)
		return nullptr;
	return &(*it);
}


const PositionedAttribute* VertexLayout::attrByRole(VertexAttributeRole role) const {
	return attrByPredicate([role](const PositionedAttribute& attr) {
		return attr.role == role;
	});
}


const PositionedAttribute* VertexLayout::attrByIndex(size32 index) const {
	if (attributeCount() > index)
		return &attrs_[index];
	return nullptr;
}



} // ns render
} // ns stardazed
