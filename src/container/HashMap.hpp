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

	MultiArrayBuffer<Key, Bucket, Value> data_;
	uint count_;


	static constexpr uint64 lower62Bits = (uint64_t)(-1) ^ (3ull << 62);
	static constexpr float growthFactor_s = 1.5f;
	
	static constexpr bool trivialDestructors = (std::is_trivially_destructible<Key>::value && std::is_trivially_destructible<Value>::value);


	template <typename F>
	void scanBuckets(uint index, F&& fn) const {
		auto bucketBase = data_.template elementsBasePtr<1>();
		auto bucketFirst = bucketBase + index;
		auto bucketEnd = bucketBase + bucketCount();

		auto bucketCur = bucketFirst;
		while (bucketCur != bucketEnd) {
			if (! fn(index++, bucketCur++))
				return;
		}
		
		bucketCur = bucketBase;
		index = 0;
		while (bucketCur != bucketFirst) {
			if (! fn(index++, bucketCur++))
				return;
		}
	}


	template <typename F>
	void withFirstFillableBucket(Bucket* bucketBase, uint bucketCount, uint index, F&& fn) {
		auto bucketFirst = bucketBase + index;
		auto bucketEnd = bucketBase + bucketCount;
		
		auto bucketCur = bucketFirst;
		while (bucketCur != bucketEnd) {
			if (bucketCur->state != BucketState::Filled) {
				fn(index, bucketCur);
				return;
			}
			
			++index;
			++bucketCur;
		}
		
		bucketCur = bucketBase;
		index = 0;
		while (bucketCur != bucketFirst) {
			if (bucketCur->state != BucketState::Filled) {
				fn(index, bucketCur);
				return;
			}
			
			++index;
			++bucketCur;
		}
		
		assert(!"Could not find a fillable bucket");
	}


	Key* keyPtrAtIndex(uint index) const {
		return data_.template elementsBasePtr<0>() + index;
	}
	
	Bucket* bucketPtrAtIndex(uint index) const {
		return data_.template elementsBasePtr<1>() + index;
	}

	Value* valuePtrAtIndex(uint index) const {
		return data_.template elementsBasePtr<2>() + index;
	}
	
	
	bool findIndex(const Key& key, uint& outIndex) const {
		auto keyHash = hash(key) & lower62Bits;
		auto firstBucket = static_cast<uint>(keyHash % bucketCount());

		bool found = false;

		scanBuckets(firstBucket,
			[this, keyHash, &key, &found, &outIndex]
			(uint index, Bucket* bucket) {
				if (bucket->state == BucketState::Filled && bucket->hash == keyHash) {
					auto testKey = keyPtrAtIndex(index);
					if (*testKey == key) {
						outIndex = index;
						found = true;
						return false;
					}
				}
				else if (bucket->state == BucketState::Empty) {
					// key not found
					return false;
				}
				
				return true;
			});
		
		return found;
	}


public:
	HashMap(memory::Allocator& allocator, uint initialCapacity)
	: data_{ allocator, initialCapacity * 3 / 2 }
	, count_(0)
	{
		assert(initialCapacity >= 64);
	}

	HashMap() : HashMap{ memory::SystemAllocator::sharedInstance(), 64 } {}
	explicit HashMap(memory::Allocator& allocator) : HashMap{ allocator, 64 } {}
	explicit HashMap(uint initialCapacity) : HashMap{ memory::SystemAllocator::sharedInstance(), initialCapacity } {}
	
	~HashMap() {
		if (! trivialDestructors) {
			clear();
		}
	}


	uint count() const { return count_; }
	bool empty() const { return count_ == 0; }
	uint bucketCount() const { return data_.capacity(); }
	
	memory::Allocator& allocator() const { return data_.allocator(); }


	void insert(const Key& key, const Value& value) {
		if (count() * 3 / 2 >= bucketCount()) {
			rehash(bucketCount() * growthFactor_s);
		}
		
		auto keyHash = hash(key) & lower62Bits;
		auto firstBucket = static_cast<uint>(keyHash % bucketCount());
		auto bucketBase = data_.template elementsBasePtr<1>();

		withFirstFillableBucket(bucketBase, bucketCount(), firstBucket,
			[this, keyHash, &key, &value]
			(uint index, Bucket* bucket) {
				bucket->hash = keyHash;
				bucket->state = BucketState::Filled;
				
				new (keyPtrAtIndex(index)) Key{ key };
				new (valuePtrAtIndex(index)) Value{ value };
			});
		
		++count_;
	}
	

	Value* find(const Key& key) const {
		uint index = 0;
		
		if (findIndex(key, index)) {
			return valuePtrAtIndex(index);
		}
		
		return nullptr;
	}
	
	
	void remove(const Key& key) {
		uint index = 0;
		
		if (findIndex(key, index)) {
			auto bucket = bucketPtrAtIndex(index);
			bucket->state = BucketState::Removed;
			bucket->hash = 0;

			if (! std::is_trivially_destructible<Key>::value) {
				auto key = keyPtrAtIndex(index);
				key->~Key();
			}

			if (! std::is_trivially_destructible<Value>::value) {
				auto value = valuePtrAtIndex(index);
				value->~Value();
			}

			--count_;
		}
	}
	
	
	void clear() {
		auto bucket = bucketPtrAtIndex(0);

		if (trivialDestructors) {
			memset(bucket, 0, sizeof(Bucket) * bucketCount());
		}
		else {
			auto key = keyPtrAtIndex(0);
			auto value = valuePtrAtIndex(0);
			
			for (uint x=0, num = bucketCount(); x < num; ++x) {
				if (bucket->state == BucketState::Filled) {
					if (! std::is_trivially_destructible<Key>::value) {
						key->~Key();
					}
					
					if (! std::is_trivially_destructible<Value>::value) {
						value->~Value();
					}
				}

				bucket->state = BucketState::Empty;
				
				++bucket;
				++key;
				++value;
			}
		}
		
		count_ = 0;
	}


	void reserve(uint newCapacity) {
		rehash(newCapacity * 3 / 2);
	}
	
	
	void rehash(uint newBucketCount) {
		auto minimumBuckets = count() * 3 / 2;
		if (newBucketCount < minimumBuckets)
			newBucketCount = minimumBuckets;

		decltype(data_) newData { data_.allocator(), newBucketCount };
		// MAB may decide to create bigger arrays for alignment purposes
		newBucketCount = newData.capacity();

		auto newKeys = newData.template elementsBasePtr<0>();
		auto newBuckets = newData.template elementsBasePtr<1>();
		auto newValues = newData.template elementsBasePtr<2>();
		
		auto bucket = bucketPtrAtIndex(0);
		auto key = keyPtrAtIndex(0);
		auto value = valuePtrAtIndex(0);

		for (uint x=0, num = bucketCount(); x < num; ++x) {
			if (bucket->state == BucketState::Filled) {
				auto keyHash = bucket->hash;
				auto firstBucket = static_cast<uint>(keyHash % newBucketCount);
				
				withFirstFillableBucket(newBuckets, newBucketCount, firstBucket,
					[=](uint index, Bucket* bk) {
						bk->hash = keyHash;
						bk->state = BucketState::Filled;
						
						new (newKeys + index) Key{ *key };
						new (newValues + index) Value{ *value };
					});
			}

			++bucket;
			++key;
			++value;
		}
		
		data_.swap(newData);
	}


	// -- ranges (experimental)
private:
	class Range {
		Key *keyPtr_, *endKeyPtr_;
		Bucket* bucketPtr_;
		Value* valuePtr_;
		
	public:
		Range(decltype(data_)& dr) {
			keyPtr_ = dr.template elementsBasePtr<0>() - 1;
			endKeyPtr_ = keyPtr_ + dr.capacity();
			bucketPtr_ = dr.template elementsBasePtr<1>() - 1;
			valuePtr_ = dr.template elementsBasePtr<2>() - 1;
		}
		
		bool next() {
			while (keyPtr_ != endKeyPtr_) {
				++keyPtr_;
				++bucketPtr_;
				++valuePtr_;
				
				if (keyPtr_ != endKeyPtr_ && bucketPtr_->state == BucketState::Filled)
					break;
			}
			return keyPtr_ != endKeyPtr_;
		}
		
		struct KeyVal {
			const Key& key;
			const Value& val;
		};
		
		KeyVal current() {
			return { *keyPtr_, *valuePtr_ };
		}
	};

public:
	Range all() {
		return { data_ };
	}
};


} // ns container


// -- export to sd namespace
using container::HashMap;


} // ns stardazed

#endif
