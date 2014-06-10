// ------------------------------------------------------------------
// memory::ObjectPool - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_MEM_OBJECTPOOL_H
#define SD_MEM_OBJECTPOOL_H

#include <vector>
#include <memory>
#include <algorithm>


namespace stardazed {


template <typename T, size_t N>
class ObjectPool {
public:
	ObjectPool()
	: items_ { std::make_unique<char[]>(sizeof(T) * N) }
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
	
	ObjectPool& operator=(ObjectPool&& other) {
		items_ = std::move(other.items_);
		first_ = std::move(other.first_);
		last_  = std::move(other.last_);
		current_ = std::move(other.current_);
		
		other.current_ = other._first_;
	}
	
	~ObjectPool() {
		std::for_each(first_, current_, [](T& p) { p.~T(); });
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
	
private:
	T* nextPtr() {
		if (current_ == last_)
			return nullptr;
		return current_++;
	}
	
	std::unique_ptr<char[]> items_;
	T* const first_;
	T* const last_;
	T* current_;
};



template <typename T, size_t N>
class ObjectPoolChain {
public:
	ObjectPoolChain() {
		appendPool();
	}
	
	ObjectPoolChain(const ObjectPoolChain&) = delete;
	ObjectPoolChain& operator=(const ObjectPoolChain&) = delete;

	ObjectPoolChain(ObjectPoolChain&& other)
	: pools_ { std::move(other.pools_) }
	, currentPool_ { other.currentPool }
	{
		other.currentPool_ = nullptr;
	}

	ObjectPoolChain& operator=(ObjectPoolChain&& other) {
		pools_ = std::move(other.pools_);
		currentPool_ = other.currentPool;

		other.currentPool_ = nullptr;
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
	
private:
	void appendPool() {
		pools_.emplace_back();
		currentPool_ = &pools_.back();
	}
	
	using PoolType = ObjectPool<T, N>;
	std::vector<PoolType> pools_;
	PoolType* currentPool_;
};


} // ns stardazed

#endif
