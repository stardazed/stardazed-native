// ------------------------------------------------------------------
// container::ObjectPool - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_CONTAINER_OBJECTPOOL_H
#define SD_CONTAINER_OBJECTPOOL_H

#include "system/Config.hpp"
#include <vector>
#include <memory>
#include <algorithm>
#include <iterator>

namespace stardazed {
namespace container {


//   ___  _     _           _   ____             _
//  / _ \| |__ (_) ___  ___| |_|  _ \ ___   ___ | |
// | | | | '_ \| |/ _ \/ __| __| |_) / _ \ / _ \| |
// | |_| | |_) | |  __/ (__| |_|  __/ (_) | (_) | |
//  \___/|_.__// |\___|\___|\__|_|   \___/ \___/|_|
//           |__/

template <typename T, size_t N>
class ObjectPool {
public:
	ObjectPool()
	: items_ ( std::unique_ptr<char[]>(new char[sizeof(T) * N]) )
	, first_ { reinterpret_cast<T*>(items_.get()) }
	, last_ { first_ + N }
	, current_ { first_ }
	{}

	ObjectPool(const ObjectPool&) = delete;
	ObjectPool& operator=(const ObjectPool&) = delete;
	
	ObjectPool(ObjectPool&& other)
	noexcept(std::is_nothrow_move_constructible<std::unique_ptr<char[]>>::value)
	: items_ { std::move(other.items_) }
	, first_ { std::move(other.first_) }
	, last_ { std::move(other.last_) }
	, current_ { std::move(other.current_) }
	{
		other.current_ = other.first_;
	}
	
	~ObjectPool() {
		std::for_each(first_, current_, [](T& r) { r.~T(); });
	}
	
	template <typename... Args>
	T* emplace(Args&&... args) {
		auto p = nextPtr();
		new (p) T(std::forward<Args>(args)...);
		return p;
	}
	
	constexpr bool available() const {
		return current_ != last_;
	}
	
	template <typename U>
	class IteratorType {
	public:
		using value_type = U;
		using difference_type = std::ptrdiff_t;
		using pointer = U*;
		using reference = U&;
		using iterator_category = std::random_access_iterator_tag;
		
		constexpr IteratorType(T* item) : item_(item) {}
		
		constexpr reference operator *() const { return *item_; }
		const IteratorType& operator ++() { ++item_; return *this; }
		IteratorType operator ++(int) { IteratorType ret(*this); ++item_; return ret; }
		
		constexpr bool operator ==(const IteratorType & rhs) const { return item_ == rhs.item_; }
		constexpr bool operator !=(const IteratorType & rhs) const { return !(*this == rhs); }
		
	private:
		T* item_;
	};
	
	using Iterator = IteratorType<T>;
	using ConstIterator = IteratorType<const T>;
	
	constexpr Iterator begin() { return { first_ }; }
	constexpr Iterator end() { return { current_ }; }
	constexpr ConstIterator begin() const { return { first_ }; }
	constexpr ConstIterator end() const { return { current_ }; }
	
private:
	constexpr T* nextPtr() {
		return current_++;
	}
	
	std::unique_ptr<char[]> items_;
	T* const first_;
	T* const last_;
	T* current_;
};


template <typename T, size_t N>
constexpr typename ObjectPool<T, N>::Iterator begin(ObjectPool<T, N>& pool) { return pool.begin(); }
template <typename T, size_t N>
constexpr typename ObjectPool<T, N>::Iterator end(ObjectPool<T, N>& pool) { return pool.end(); }



//   ___  _     _           _   ____             _  ____ _           _
//  / _ \| |__ (_) ___  ___| |_|  _ \ ___   ___ | |/ ___| |__   __ _(_)_ __
// | | | | '_ \| |/ _ \/ __| __| |_) / _ \ / _ \| | |   | '_ \ / _` | | '_ \
// | |_| | |_) | |  __/ (__| |_|  __/ (_) | (_) | | |___| | | | (_| | | | | |
//  \___/|_.__// |\___|\___|\__|_|   \___/ \___/|_|\____|_| |_|\__,_|_|_| |_|
//           |__/

template <typename T, size_t N>
class ObjectPoolChain {
	using PoolType = ObjectPool<T, N>;
	template <typename U>
	using PoolIteratorType = typename PoolType::template IteratorType<U>;
	
	using ChainType = std::vector<PoolType>;
	using ChainIterator = typename ChainType::iterator;
	
public:
	ObjectPoolChain() {
		appendPool();
	}
	
	ObjectPoolChain(const ObjectPoolChain&) = delete;
	ObjectPoolChain& operator=(const ObjectPoolChain&) = delete;
	
	ObjectPoolChain(ObjectPoolChain&& other)
	noexcept(std::is_nothrow_move_constructible<std::vector<PoolType>>::value)
	: pools_ { std::move(other.pools_) }
	, currentPool_ { std::move(other.currentPool_) }
	{}
	
	ObjectPoolChain& operator=(ObjectPoolChain&& other) {
		pools_ = std::move(other.pools_);
		currentPool_ = std::move(other.currentPool_);
	}
	
	template <typename... Args>
	T* emplace(Args&&... args) {
		if (! currentPool_->available())
			appendPool();
		
		return currentPool_->emplace(std::forward<Args>(args)...);
	}
	
	constexpr bool available() const {
		return true;
	}
	
	template <typename U>
	class IteratorType {
	public:
		// typedefs defined by std::iterator
		using value_type = U;
		using difference_type = std::ptrdiff_t;
		using pointer = U*;
		using reference = U&;
		using iterator_category = std::forward_iterator_tag;
		
		constexpr IteratorType(ChainIterator poolIt, ChainIterator poolEndIt, PoolIteratorType<U> itemIt)
		: poolIt_(poolIt), poolEndIt_(poolEndIt), itemIt_(itemIt) {}
		
		constexpr reference operator *() const { return *itemIt_; }
		const IteratorType& operator ++() {
			++itemIt_;
			
			if (itemIt_ == poolIt_->end()) {
				++poolIt_;
				if (poolIt_ != poolEndIt_) {
					itemIt_ = poolIt_->begin();
				}
			}
			return *this;
		}
		
		IteratorType operator ++(int) { IteratorType ret(*this); ++(*this); return ret; }
		
		constexpr bool operator ==(const IteratorType & rhs) const { return itemIt_ == rhs.itemIt_; }
		constexpr bool operator !=(const IteratorType & rhs) const { return !(*this == rhs); }
		
	private:
		ChainIterator poolIt_, poolEndIt_;
		PoolIteratorType<U> itemIt_;
	};
	
	using Iterator = IteratorType<T>;
	using ConstIterator = IteratorType<const T>;
	
	constexpr Iterator begin() { return { pools_.begin(), std::next(currentPool_), pools_.begin()->begin() }; }
	constexpr Iterator end() { return { currentPool_, std::next(currentPool_), currentPool_->end() }; }
	constexpr ConstIterator begin() const { return { pools_.begin(), std::next(currentPool_), pools_.begin()->begin() }; }
	constexpr ConstIterator end() const { return { currentPool_, std::next(currentPool_), currentPool_->end() }; }
	
private:
	void appendPool() {
		pools_.push_back(PoolType{});
		currentPool_ = std::prev(std::end(pools_));
	}
	
	ChainType pools_;
	ChainIterator currentPool_;
};


template <typename T, size_t N>
constexpr typename ObjectPoolChain<T, N>::Iterator begin(ObjectPoolChain<T, N>& chain) { return chain.begin(); }
template <typename T, size_t N>
constexpr typename ObjectPoolChain<T, N>::Iterator end(ObjectPoolChain<T, N>& chain) { return chain.end(); }


} // ns container
} // ns stardazed

#endif
