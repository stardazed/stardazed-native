// ------------------------------------------------------------------
// memory::ObjectPool - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_MEM_OBJECTPOOL_H
#define SD_MEM_OBJECTPOOL_H

#include <vector>
#include <memory>
#include <algorithm>
#include <iterator>


namespace stardazed {


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
		return *this;
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

public:
	ObjectPoolChain() {
		appendPool();
	}
	
	ObjectPoolChain(const ObjectPoolChain&) = delete;
	ObjectPoolChain& operator=(const ObjectPoolChain&) = delete;

	ObjectPoolChain(ObjectPoolChain&& other)
	noexcept(std::is_nothrow_move_constructible<std::vector<PoolType>>::value)
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
	
	std::vector<PoolType> pools_;
	PoolType* currentPool_;
};


} // ns stardazed

#endif
