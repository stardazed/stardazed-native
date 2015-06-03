// ------------------------------------------------------------------
// render::TriangleBuffer - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_TRIANGLEBUFFER_H
#define SD_RENDER_TRIANGLEBUFFER_H

#include "system/Config.hpp"
#include "util/ConceptTraits.hpp"
#include "math/Vector.hpp"
#include "render/common/IndexBuffer.hpp"

#include <memory>

namespace stardazed {
namespace render {


class TriangleBuffer {
	IndexElementType indexElementType_ = IndexElementType::UInt16;
	size32 triangleSizeBytes_ = 0, triangleCount_ = 0;
	std::unique_ptr<uint8[]> storage_;
	
public:
	void allocateWithIndexElementType(IndexElementType, size32 triangleCount);
	void allocateWithVertexCount(size32 vertexCount, size32 triangleCount);

	// -- observers
	
	void* basePointer() const { return storage_.get(); }
	
	IndexElementType indexElementType() const { return indexElementType_; }
	
	size32 triangleSizeBytes() const { return triangleSizeBytes_; }
	size32 bufferSizeBytes() const { return triangleSizeBytes_ * triangleCount_; }

	size32 triangleCount() const { return triangleCount_; }
	size32 indexCount() const { return triangleCount_ * 3; }


	// -- iterators
	
	class TriangleProxy {
		uint8* data_;
		size32 elementSize_;
		
	public:
		constexpr TriangleProxy(uint8* data, size32 elementSize)
		: data_(data)
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


	class TriangleIterator
	: public std::iterator<std::random_access_iterator_tag, TriangleProxy>
	, public FullyComparableTrait<TriangleIterator>
	{
		uint8* position_ = nullptr;
		size32 indexSizeBytes_ = 0, triangleSizeBytes_ = 0;
	
	public:
		constexpr TriangleIterator() {}
		constexpr TriangleIterator(const TriangleBuffer& tb)
		: position_{ static_cast<uint8*>(tb.basePointer()) }
		, indexSizeBytes_{ indexElementTypeSizeBytes(tb.indexElementType()) }
		, triangleSizeBytes_{ tb.triangleSizeBytes() }
		{}
		
		constexpr TriangleProxy operator *() { return { position_, indexSizeBytes_ }; }
//		constexpr TriangleProxy* operator ->() { return reinterpret_cast<NativeFieldType*>(position_); }
		
		constexpr TriangleProxy operator [](size32 index) {
			auto indexedPos = position_ + (triangleSizeBytes_ * index);
			return { indexedPos, indexSizeBytes_ };
		}
		
		constexpr const TriangleProxy operator [](size32 index) const {
			auto indexedPos = position_ + (triangleSizeBytes_ * index);
			return { indexedPos, indexSizeBytes_ };
		}

		constexpr const TriangleIterator& operator ++() { position_ += triangleSizeBytes_; return *this; }
		constexpr TriangleIterator operator ++(int) { auto ret = *this; position_ += triangleSizeBytes_; return ret; }
		
		constexpr const TriangleIterator& operator --() { position_ -= triangleSizeBytes_; return *this; }
		constexpr TriangleIterator operator --(int) { auto ret = *this; position_ -= triangleSizeBytes_; return ret; }
		
		constexpr bool operator ==(const TriangleIterator& other) const { return position_ == other.position_; }
		constexpr bool operator <(const TriangleIterator& other) const { return position_ < other.position_; }

		friend constexpr TriangleIterator operator +(const TriangleIterator& iter, size32 count) {
			auto ret = iter;
			ret.position_ += ret.triangleSizeBytes_ * count;
			return ret;
		}
		
		friend constexpr TriangleIterator operator +(size32 count, const TriangleIterator& iter) {
			auto ret = iter;
			ret.position_ += ret.triangleSizeBytes_ * count;
			return ret;
		}
		
		friend constexpr TriangleIterator operator -(const TriangleIterator& iter, size32 count) {
			auto ret = iter;
			ret.position_ -= ret.triangleSizeBytes_ * count;
			return ret;
		}
		
		friend TriangleIterator& operator +=(TriangleIterator& iter, size32 count) {
			iter.position_ += iter.triangleSizeBytes_ * count;
			return iter;
		}
		
		friend TriangleIterator& operator -=(TriangleIterator& iter, size32 count) {
			iter.position_ -= iter.triangleSizeBytes_ * count;
			return iter;
		}
		
		constexpr ptrdiff_t operator -(const TriangleIterator& b) {
			return (position_ - b.position_) / static_cast<ptrdiff_t>(triangleSizeBytes_);
		}
	};
	
	TriangleIterator begin() const { return { *this }; }
	TriangleIterator end() const { return begin() + triangleCount_; }
};


} // ns render
} // ns stardazed

#endif
