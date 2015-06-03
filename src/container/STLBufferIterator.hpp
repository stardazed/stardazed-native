// ------------------------------------------------------------------
// container::STLBufferIterator - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_CONTAINER_STLBUFFERITERATOR_H
#define SD_CONTAINER_STLBUFFERITERATOR_H

#include "system/Config.hpp"
#include "util/ConceptTraits.hpp"
#include <iterator>

namespace stardazed {
namespace container {


// A generic STL iterator that can be used to iterate over values in any
// data buffer with a custom stride.

// By abstracting out currentValue() this iterator can support proxy objects
// as ValueType. currentValue() must be implemented using CRTP because templates.

// You can have stateful ProxyGen instances by passing the initializer parameters
// at the end of the STLBufferIterator constructor. They will be forwarded to
// the constructor of the ProxyGen.

// May the Ancient Ones have mercy on my soul.


template <typename Value>
struct STLBasicProxyGen {
	using ValueType = Value;
	using ValueRef = Value&;
	
	ValueRef valueRef(void* position) {
		return *(static_cast<ValueType*>(position));
	}
};


template <typename ProxyGen>
class STLBufferIterator
: public std::iterator<std::random_access_iterator_tag, typename ProxyGen::ValueType>
, public FullyComparableTrait<STLBufferIterator<ProxyGen>>
{
protected:
	uint8* position_ = nullptr;
	size32 strideBytes_ = 0;
	ProxyGen proxyGen_ {};

public:
	using ValueType = typename ProxyGen::ValueType;
	using ValueRef = typename ProxyGen::ValueRef;
	
	constexpr STLBufferIterator() = default;
	
	template <typename... ProxyArgs>
	constexpr STLBufferIterator(void* basePointer, size32 strideBytes, ProxyArgs... args)
	: position_{ static_cast<uint8*>(basePointer) }
	, strideBytes_{ strideBytes }
	, proxyGen_(std::forward<ProxyArgs>(args)...)
	{}

	constexpr ValueRef operator *() { return proxyGen_.valueRef(position_); }
	constexpr ValueType* operator ->() { return &proxyGen_.valueRef(position_); };

	constexpr ValueRef operator [](size32 index) {
		auto indexedPos = position_ + (strideBytes_	* index);
		return *reinterpret_cast<ValueType*>(indexedPos);
	}
	
	constexpr const ValueRef operator [](size32 index) const {
		auto indexedPos = position_ + (strideBytes_ * index);
		return const_cast<const ValueRef>(*reinterpret_cast<const ValueType*>(indexedPos));
	}

	const STLBufferIterator& operator ++() { position_ += strideBytes_; return *this; }
	STLBufferIterator operator ++(int) { auto ret = *this; position_ += strideBytes_; return ret; }
	
	const STLBufferIterator& operator --() { position_ -= strideBytes_; return *this; }
	STLBufferIterator operator --(int) { auto ret = *this; position_ -= strideBytes_; return ret; }
	
	constexpr bool operator ==(const STLBufferIterator& other) const { return position_ == other.position_; }
	constexpr bool operator <(const STLBufferIterator& other) const { return position_ < other.position_; }

	friend constexpr STLBufferIterator operator +(const STLBufferIterator& iter, uint32 count) {
		auto ret = iter;
		ret.position_ += ret.strideBytes_ * count;
		return ret;
	}
	
	friend constexpr STLBufferIterator operator +(uint32 count, const STLBufferIterator& iter) {
		auto ret = iter;
		ret.position_ += ret.strideBytes_ * count;
		return ret;
	}
	
	friend constexpr STLBufferIterator operator -(const STLBufferIterator& iter, uint32 count) {
		auto ret = iter;
		ret.position_ -= ret.strideBytes_ * count;
		return ret;
	}
	
	friend STLBufferIterator& operator +=(STLBufferIterator& iter, uint32 count) {
		iter.position_ += iter.strideBytes_ * count;
		return iter;
	}
	
	friend STLBufferIterator& operator -=(STLBufferIterator& iter, uint32 count) {
		iter.position_ -= iter.strideBytes_ * count;
		return iter;
	}
	
	constexpr ptrdiff_t operator -(const STLBufferIterator& b) {
		return (position_ - b.position_) / static_cast<ptrdiff_t>(strideBytes_);
	}
};


// -- use this one for non-proxied values (most cases)
template <typename Value>
using STLBasicBufferIterator = STLBufferIterator<STLBasicProxyGen<Value>>;


} // ns container
} // ns stardazed

#endif
