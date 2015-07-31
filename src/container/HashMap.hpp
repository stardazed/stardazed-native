// ------------------------------------------------------------------
// container::HashMap - stardazed
// (c) 2015 by Arthur Langereis
// Based on the DO2 hashmap implementation by Nathan Reed
// http://www.reedbeta.com/blog/2015/01/12/data-oriented-hash-table
// ------------------------------------------------------------------

#ifndef SD_CONTAINER_HASHMAP_H
#define SD_CONTAINER_HASHMAP_H

#include "system/Config.hpp"
#include "container/MultiArrayBuffer.hpp"
#include "util/Hash.hpp"

#include <type_traits>

namespace stardazed {
namespace container {


template <typename Key, typename Value>
class HashMap {
	enum class BucketState : uint64 {
		Empty,
		Filled,
		Removed
	};
	
	struct Bucket {
		uint64 hash:62;
		BucketState state:2;
	};

	MultiArrayBuffer<
		Key,
		Bucket,
		Value,
	> data_;
	
	static constexpr uint32 initialCapacity = 96; // 64 / 0.667 max load factor

public:
	HashMap(memory::Allocator& allocator)
	: data_{ allocator, initialCapacity }
	{}
	
	void insert(const Key& key, const Value& value) {
		
	}
};


} // ns container


// -- export to sd namespace
using container::HashMap;


} // ns stardazed

#endif
