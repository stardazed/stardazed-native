// ------------------------------------------------------------------
// render::VertexBuffer - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "render/common/VertexBuffer.hpp"

#include <algorithm>

namespace stardazed {
namespace render {


constexpr size32_t alignFieldOnSize(size32_t size, size32_t offset) {
	// FIXME: this will fail if size is not a power of 2
	// extend to nearest power of 2, then - 1
	size32_t mask = size - 1;
	return (offset + mask) & ~mask;
}


constexpr size32_t alignFieldOnElement(ElementType element, size32_t offset) {
	return alignFieldOnSize(elementSize(element), offset);
}


VertexBuffer::VertexBuffer(const AttributeList& attrList) {
	attrs_.reserve(attrList.size());
	size32_t offset = 0, maxElemSize = 0;

	// calculate positioning of successive attributes in linear item
	std::transform(begin(attrList), end(attrList), std::back_inserter(attrs_),
		[&offset, &maxElemSize](const Attribute& attr) -> PositionedAttribute {
			auto field = getField(attr);
			auto size = fieldSize(field);
			maxElemSize = std::max(maxElemSize, elementSize(field.type));

			auto alignedOffset = alignFieldOnElement(field.type, offset);
			offset = alignedOffset + size;
			return { attr, alignedOffset };
		});

	// align full item size on boundary of biggest element in attribute list, with min of float boundary
	maxElemSize = std::max(elementSize(ElementType::Float), maxElemSize);
	itemSizeBytes_ = alignFieldOnSize(maxElemSize, offset);
}


const PositionedAttribute* VertexBuffer::attrByPredicate(std::function<bool(const PositionedAttribute&)> pred) const {
	auto it = std::find_if(begin(attrs_), end(attrs_), pred);

	if (it == end(attrs_))
		return nullptr;
	return &(*it);
}


const PositionedAttribute* VertexBuffer::attrByRole(AttributeRole role) const {
	return attrByPredicate([role](const PositionedAttribute& attr) {
		return attr.attr.role == role;
	});
}


const PositionedAttribute* VertexBuffer::attrByName(const std::string& name) const {
	return attrByPredicate([&name](const PositionedAttribute& attr) {
		return attr.attr.namedField.name == name;
	});
}


const PositionedAttribute* VertexBuffer::attrByIndex(size32_t index) const {
	if (attrs_.size() > index)
		return &attrs_[index];
	return nullptr;
}


} // ns render
} // ns stardazed
