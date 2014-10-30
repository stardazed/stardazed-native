// ------------------------------------------------------------------
// render::VertexBuffer - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_VERTEXBUFFER_H
#define SD_RENDER_VERTEXBUFFER_H

#include "system/Config.hpp"
#include "util/ConceptTraits.hpp"
#include "render/common/BufferFields.hpp"
#include "math/Vector.hpp"
#include "math/Matrix.hpp"

#include <string>
#include <vector>
#include <memory>
#include <functional>

namespace stardazed {
namespace render {


enum class AttributeRole : uint16_t {
	Generic,
	Position,
	Normal,
	Tangent,
	Colour,
	UV,
	UVW,
	Index
};


struct Attribute {
	NamedField namedField;
	AttributeRole role;
};


using AttributeList = std::vector<Attribute>;


struct PositionedAttribute {
	Attribute attr;
	size32_t offset;
};


constexpr Field getField(const Attribute& attr) { return attr.namedField.field; }
constexpr Field getField(const PositionedAttribute& posAttr) { return posAttr.attr.namedField.field; }


class VertexBuffer {
	std::vector<PositionedAttribute> attrs_;
	size32_t itemSizeBytes_, itemCount_;
	std::unique_ptr<uint8_t[]> data_;
	
	const PositionedAttribute* attrByPredicate(std::function<bool(const PositionedAttribute&)>) const;

public:
	VertexBuffer(const AttributeList&, size32_t itemCount);

	size32_t itemSizeBytes() const { return itemSizeBytes_; }

	const PositionedAttribute* attrByRole(AttributeRole) const;
	const PositionedAttribute* attrByName(const std::string&) const;
	const PositionedAttribute* attrByIndex(size32_t) const;

	// -- iteration over attribute data

	template <typename NativeFieldType>
	class AttrIterator
	: public std::iterator<std::random_access_iterator_tag, NativeFieldType>
	, public FullyComparableTrait<AttrIterator<NativeFieldType>>
	{
		const VertexBuffer& vb_;
		const PositionedAttribute& attr_;
		uint8_t* position_;
		size32_t rowBytes_;
		
	public:
		constexpr AttrIterator(const VertexBuffer& vb, const PositionedAttribute& attr)
		: vb_{vb}, attr_{attr}
		, position_{ vb.data_.get() + attr.offset }
		, rowBytes_{ vb.itemSizeBytes() }
		{}
		
		constexpr NativeFieldType& operator *() { return *reinterpret_cast<NativeFieldType*>(position_); }
		constexpr NativeFieldType* operator ->() { return reinterpret_cast<NativeFieldType*>(position_); }
		
		constexpr NativeFieldType& operator [](size32_t index) {
			auto indexedPos = position_ + (rowBytes_ * index);
			return *reinterpret_cast<NativeFieldType*>(indexedPos);
		}

		constexpr const AttrIterator& operator ++() { position_ += rowBytes_; return *this; }
		constexpr AttrIterator operator ++(int) { auto ret = *this; position_ += rowBytes_; return ret; }
		
		constexpr const AttrIterator& operator --() { position_ -= rowBytes_; return *this; }
		constexpr AttrIterator operator --(int) { auto ret = *this; position_ -= rowBytes_; return ret; }
		
		constexpr bool operator ==(const AttrIterator& other) { return position_ == other.position_; }
		constexpr bool operator <(const AttrIterator& other) { return position_ < other.position_; }
		
		friend constexpr AttrIterator operator +(const AttrIterator&, size32_t);
		friend constexpr AttrIterator operator +(size32_t, const AttrIterator&);
		friend constexpr AttrIterator operator -(const AttrIterator&, size32_t);

		friend AttrIterator& operator +=(AttrIterator&, size32_t);
		friend AttrIterator& operator -=(AttrIterator&, size32_t);
	};
	
	template <typename T>
	friend class AttrIterator;
	
	template <typename T>
	AttrIterator<T> attrBegin(const PositionedAttribute& attr) { return { *this, attr }; }

	template <typename T>
	AttrIterator<T> attrEnd(const PositionedAttribute& attr) {
		auto it = attrBegin<T>(attr);
		return std::next(it, itemCount_);
	}
};


template <typename T>
constexpr VertexBuffer::AttrIterator<T> operator +(const VertexBuffer::AttrIterator<T>& iter, size32_t count) {
	auto ret = iter;
	ret.position_ += ret.rowBytes_ * count;
	return ret;
}


template <typename T>
constexpr VertexBuffer::AttrIterator<T> operator +(size32_t count, const VertexBuffer::AttrIterator<T>& iter) {
	auto ret = iter;
	ret.position_ += ret.rowBytes_ * count;
	return ret;
}

template <typename T>
constexpr VertexBuffer::AttrIterator<T> operator -(const VertexBuffer::AttrIterator<T>& iter, size32_t count) {
	auto ret = iter;
	ret.position_ -= ret.rowBytes_ * count;
	return ret;
}

template <typename T>
VertexBuffer::AttrIterator<T>& operator +=(VertexBuffer::AttrIterator<T>& iter, size32_t count) {
	iter.position_ += iter.rowBytes_ * count;
	return iter;
}

template <typename T>
VertexBuffer::AttrIterator<T>& operator -=(VertexBuffer::AttrIterator<T>& iter, size32_t count) {
	iter.position_ -= iter.rowBytes_ * count;
	return iter;
}


} // ns render
} // ns stardazed

#endif
