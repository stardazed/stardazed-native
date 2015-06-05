// ------------------------------------------------------------------
// render::VertexBuffer - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_VERTEXBUFFER_H
#define SD_RENDER_VERTEXBUFFER_H

#include "system/Config.hpp"
#include "util/ConceptTraits.hpp"
#include "container/STLBufferIterator.hpp"
#include "render/common/VertexLayout.hpp"
#include "math/Vector.hpp"
#include "math/Matrix.hpp"

#include <memory>

namespace stardazed {
namespace render {


class VertexBuffer {
	VertexLayout layout_;
	size32 itemCount_ = 0;
	std::unique_ptr<uint8[]> storage_;

public:
	VertexBuffer(const VertexAttributeList&);
	VertexBuffer(const VertexLayout&);

	// -- buffer data management

	const VertexLayout& layout() const { return layout_; }
	size32 strideBytes() const { return layout_.vertexSizeBytes(); }
	size32 attributeCount() const { return layout_.attributeCount(); }
	size32 itemCount() const { return itemCount_; }
	size32 bufferSizeBytes() const { return strideBytes() * itemCount_; }

	void allocate(size32 itemCount);

	// -- attribute access pass-through
	
	bool hasAttributeWithRole(VertexAttributeRole role) const {
		return layout_.hasAttributeWithRole(role);
	}
	const PositionedAttribute* attrByRole(VertexAttributeRole role) const {
		return layout_.attrByRole(role);
	}
	const PositionedAttribute* attrByIndex(size32 index) const {
		return layout_.attrByIndex(index);
	}
	
	// -- raw data pointers
	
	void* basePointer() const { return storage_.get(); }
	
	void* attrBasePointer(const PositionedAttribute&) const;
	void* attrBasePointer(VertexAttributeRole) const;

	// -- iteration over attribute data

	template <typename NativeFieldType>
	using AttrIterator = container::STLBasicBufferIterator<NativeFieldType>;
	
	template <typename T>
	container::STLBasicBufferIterator<T> attrBegin(const PositionedAttribute& attr) const {
		return { static_cast<uint8*>(basePointer()) + attr.offset, strideBytes() };
	}
	template <typename T>
	container::STLBasicBufferIterator<T> attrEnd(const PositionedAttribute& attr) const {
		return attrBegin<T>(attr) + itemCount_;
	}
	
	template <typename T>
	container::STLBasicBufferIterator<T> attrBegin(VertexAttributeRole role) const {
		return attrBegin<T>(*layout_.attrByRole(role));
	}
	template <typename T>
	container::STLBasicBufferIterator<T> attrEnd(VertexAttributeRole role) const {
		return attrEnd<T>(*layout_.attrByRole(role));
	}
};


} // ns render
} // ns stardazed

#endif
