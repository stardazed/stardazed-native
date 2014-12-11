// ------------------------------------------------------------------
// render::IndexBuffer - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_INDEXBUFFER_H
#define SD_RENDER_INDEXBUFFER_H

#include "system/Config.hpp"
#include "math/Vector.hpp"
#include "render/common/VertexBuffer.hpp"

#include <memory>

namespace stardazed {
namespace render {


using TriangleI16 = math::Vector<3, uint16_t>;
using TriangleI32 = math::Vector<3, uint32_t>;


namespace detail {
	template <ElementType IndexType>
	struct TriangleTypeForIndexType;
	
	template <>
	struct TriangleTypeForIndexType<ElementType::UInt16> {
		using Triangle = TriangleI16;
	};

	template <>
	struct TriangleTypeForIndexType<ElementType::UInt32> {
		using Triangle = TriangleI32;
	};
}


template <ElementType IndexType>
class IndexBuffer {
	VertexBuffer indexBuffer_;
	const PositionedAttribute *indexAttr_;
	
public:
	using TriangleType = typename detail::TriangleTypeForIndexType<IndexType>::Triangle;

	IndexBuffer()
	: indexBuffer_({
		{ { { IndexType, 3 }, std::string{"index"} }, AttributeRole::Index }
	})
	, indexAttr_(indexBuffer_.attrByIndex(0))
	{}
	
	// --
	
	size32 bytesRequired(size32 triangleCount) const {
		return indexBuffer_.bytesRequired(triangleCount);
	}
	
	void allocate(size32 triangleCount) {
		indexBuffer_.allocate(triangleCount);
	}
	
	// --
	
	constexpr ElementType elementType() const { return IndexType; }
	size32 count() const { return indexBuffer_.itemCount(); }

	// --
	
	VertexBuffer::AttrIterator<TriangleType> begin() {
		return indexBuffer_.attrBegin<TriangleType>(*indexAttr_);
	}
	
	VertexBuffer::AttrIterator<TriangleType> end() {
		return indexBuffer_.attrEnd<TriangleType>(*indexAttr_);
	}
};


// -- Aliases

using Index16Buffer = IndexBuffer<ElementType::UInt16>;
using Index32Buffer = IndexBuffer<ElementType::UInt32>;


} // ns render
} // ns stardazed

#endif
