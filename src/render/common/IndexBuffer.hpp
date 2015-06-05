// ------------------------------------------------------------------
// render::IndexBuffer - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_INDEXBUFFER_H
#define SD_RENDER_INDEXBUFFER_H

#include "system/Config.hpp"
#include "util/ConceptTraits.hpp"
#include "container/STLBufferIterator.hpp"
#include "math/Vector.hpp"

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
	uint32 indexCount_ = 0, primitiveCount_ = 0, indexElementSizeBytes_ = 0;
	std::unique_ptr<uint8[]> storage_;

public:
	void allocate(PrimitiveType, IndexElementType, uint32 primitiveCount);

	// -- observers
	PrimitiveType primitiveType() const { return primitiveType_; }
	IndexElementType indexElementType() const { return indexElementType_; }

	uint32 primitiveCount() const { return primitiveCount_; }
	uint32 indexCount() const { return indexCount_; }
	uint32 indexElementSizeBytes() const { return indexElementSizeBytes_; }

	size32 bufferSizeBytes() const { return indexCount() * indexElementSizeBytes(); }
	void* basePointer() const { return storage_.get(); }

	// -- read/write indexes
	void indexes(uint32 baseIndexNr, uint32 outputCount, uint32* outputPtr) const;
	uint32 index(uint32 indexNr) const {
		uint32 result = 0;
		indexes(indexNr, 1, &result);
		return result;
	}

	void setIndexes(uint32 baseIndexNr, size32 sourceCount, const uint32* sourcePtr);
	void setIndex(uint32 indexNr, uint32 newValue) {
		setIndexes(indexNr, 1, &newValue);
	}
	
	// -- STL interop
	template <typename IndexNativeType>
	container::STLBasicBufferIterator<IndexNativeType> begin() const {
		return { basePointer(), indexElementSizeBytes() };
	}

	template <typename IndexNativeType>
	container::STLBasicBufferIterator<IndexNativeType> end() const {
		return begin<IndexNativeType>() + indexCount();
	}
};


using Triangle = math::Vector<3, uint32>;

class IndexBufferTriangleView {
	IndexBuffer& indexBuffer_;

public:
	IndexBufferTriangleView(IndexBuffer&);
	
	uint32 triangleCount() const {
		return indexBuffer_.primitiveCount();
	}
	
	void setTriangle(uint32 triangleIndex, const Triangle& tri);
	void setTriangle(uint32 triangleIndex, uint32 a, uint32 b, uint32 c) {
		setTriangle(triangleIndex, { a, b, c });
	}

	Triangle triangleAtIndex(uint32) const;

	// -- STL interop
	class TriangleProxy {
		uint8* data_;
		size32 elementSize_;
		
	public:
		constexpr TriangleProxy(void* data, size32 elementSize)
		: data_(static_cast<uint8*>(data))
		, elementSize_(elementSize)
		{}

		constexpr uint32 index(size32 index) const {
			if (elementSize_ == 2) {
				auto fieldPtr = reinterpret_cast<uint16*>(data_);
				return fieldPtr[index];
			}
			else if (elementSize_ == 4) {
				auto fieldPtr = reinterpret_cast<uint32*>(data_);
				return fieldPtr[index];
			}
			
			return data_[index];
		}
		
		void setIndex(size32 index, uint32 value) {
			if (elementSize_ == 2) {
				auto fieldPtr = reinterpret_cast<uint16*>(data_);
				fieldPtr[index] = value;
			}
			else if (elementSize_ == 4) {
				auto fieldPtr = reinterpret_cast<uint32*>(data_);
				fieldPtr[index] = value;
			}
			else {
				data_[index] = value;
			}
		}
		
		constexpr uint32 a() const { return index(0); }
		constexpr uint32 b() const { return index(1); }
		constexpr uint32 c() const { return index(2); }
		
		void setA(uint32 value) { setIndex(0, value); }
		void setB(uint32 value) { setIndex(1, value); }
		void setC(uint32 value) { setIndex(2, value); }
		
		TriangleProxy& operator =(const Triangle& t) {
			setA(t.x);
			setB(t.y);
			setC(t.z);
			return *this;
		}

		TriangleProxy& operator =(const TriangleProxy& tp) {
			setA(tp.a());
			setB(tp.b());
			setC(tp.c());
			return *this;
		}
	};

	class TriangleProxyGen {
		size32 indexElementSizeBytes_;

	public:
		using ValueType = TriangleProxy;
		using ValueRef = TriangleProxy; // proxy, not a reference type

		TriangleProxyGen(size32 indexElementSizeBytes)
		: indexElementSizeBytes_(indexElementSizeBytes)
		{}
		
		TriangleProxy valueRef(void* position) {
			return { position, indexElementSizeBytes_ };
		}
	};
	
	using TriangleIterator = container::STLBufferIterator<TriangleProxyGen>;
	
	TriangleIterator begin() const {
		return { indexBuffer_.basePointer(), indexBuffer_.indexElementSizeBytes() * 3, indexBuffer_.indexElementSizeBytes() };
	}

	TriangleIterator end() const {
		return begin() + triangleCount();
	}
};


} // ns render
} // ns stardazed

#endif
