// ------------------------------------------------------------------
// container::Array - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_CONTAINER_ARRAY_H
#define SD_CONTAINER_ARRAY_H

#include "system/Config.hpp"
#include "math/Algorithm.hpp"
#include "memory/Allocator.hpp"
#include "container/Algorithm.hpp"

#include <type_traits>
#include <initializer_list>

namespace stardazed {
namespace container {


template <typename T>
// requires DefaultConstructible<T> for resize(newCount) where newCount > count() and emplaceBack()
// prefers TriviallyDefaultConstructible<T> && TriviallyDestructible<T>
class Array {
	static constexpr bool canSkipElementConstructor() { return std::is_trivially_default_constructible<T>::value; }
	static constexpr bool canSkipElementDestructor() { return std::is_trivially_destructible<T>::value; }
	
	static constexpr float growthFactor_s = 1.5;

	memory::Allocator& allocator_;
	uint capacity_ = 0, count_ = 0;
	T* data_ = nullptr;
	

	void destructRange(T* first, uint count) {
		// Call destructor for each deleted value for Ts that need it.
		while (count--) {
			first->~T();
			++first;
		}
	}
	

	template <typename It>
	void importRange(T* dst, It src, uint elementsToImport) {
		while (elementsToImport--) {
			new (dst) T{ std::move(*src) }; // move-construct element in new array
			++src;
			++dst;
		}
		
		count_ = capacity_;
	}


public:
	Array(memory::Allocator& allocator, uint initialCapacity)
	: allocator_(allocator)
	{
		reserve(initialCapacity);
	}
	
	Array() : Array{memory::SystemAllocator::sharedInstance(), 2} {}
	
	static Array withCapacity(uint initialCapacity) {
		return { memory::SystemAllocator::sharedInstance(), initialCapacity };
	}
	
	
	Array(std::initializer_list<T> vals)
	: Array(memory::SystemAllocator::sharedInstance(), static_cast<uint32>(vals.size()))
	{
		importRange(data_, vals.begin(), capacity());
		count_ = capacity_;
	}
	
	
	Array(memory::Allocator& allocator, std::initializer_list<T> vals)
	: Array(allocator, static_cast<uint32>(vals.size()))
	{
		importRange(data_, vals.begin(), capacity());
		count_ = capacity_;
	}


	Array(const Array& rhs)
	: allocator_(rhs.allocator_)
	{
		count_ = rhs.count();
		reserve(math::max(count_, 2u));
		
		if (std::is_trivially_copy_constructible<T>::value) {
			memcpy(data_, rhs.elementsBasePtr(), rhs.count() * elementSizeBytes());
		}
		else {
			auto elementsToCopy = rhs.count();
			auto rhsElementPtr = rhs.elementsBasePtr();
			auto newElementPtr = data_;

			while (elementsToCopy--) {
				new (newElementPtr) T(*rhsElementPtr);
				++newElementPtr;
				++rhsElementPtr;
			}
		}
	}


	Array(Array&& rhs)
	: allocator_(rhs.allocator_)
	{
		capacity_ = rhs.capacity();
		count_ = rhs.count();
		data_ = rhs.elementsBasePtr();

		rhs.capacity_ = 0;
		rhs.count_ = 0;
		rhs.data_ = nullptr;
	}

	
	Array& operator =(const Array& rhs) {
		if (&rhs != this) {
			if (! canSkipElementDestructor()) {
				destructRange(data_, count());
			}

			count_ = 0;
			auto newCount = rhs.count_;
			reserve(math::max(newCount, 2u));
			
			if (newCount > 0) {
				count_ = newCount;

				if (canSkipElementConstructor()) {
					memcpy(data_, rhs.data_, count_ * elementSizeBytes());
				}
				else {
					auto myElem = data_;
					auto rhsElem = rhs.data_;

					while (newCount--) {
						new (myElem) T{ *rhsElem };
						++myElem;
						++rhsElem;
					}
				}
			}
		}
		
		return *this;
	}
	
	
	Array& operator =(Array&& rhs) {
		// FIXME: stl does optional alloc propagation, for now I just require them
		// to be the same
		assert(&allocator_ == &rhs.allocator_);
		
		std::swap(capacity_, rhs.capacity_);
		std::swap(count_, rhs.count_);
		std::swap(data_, rhs.data_);

		return *this;
	}


	~Array() {
		if (data_) {
			if (! canSkipElementDestructor()) {
				destructRange(data_, count());
			}
		
			allocator_.free(data_);
		}
	}
	

	// -- observers

	static constexpr size32 elementSizeBytes() { return sizeof32<T>(); }
	
	uint capacity() const { return capacity_; }
	uint count() const { return count_; }
	uint empty() const { return count_ == 0; }
	
	const T* elementsBasePtr() const { return const_cast<const T*>(data_); }
	T* elementsBasePtr() { return data_; }
	
	memory::Allocator& allocator() const { return allocator_; }
	

	// -- storage sizing and object lifetime

	void reserve(uint newCapacity) {
		assert(newCapacity > 0);

		if (newCapacity <= capacity()) {
			return;
		}
		
		auto newSizeBytes = newCapacity * elementSizeBytes();
		
		auto newData = static_cast<T*>(allocator_.alloc(newSizeBytes));
		assert(newData);
		
		// Not all Allocators return zero-filled memory but this container guarantees
		// zeroed elements for default constructed values so we pre-clear the memory
		if (canSkipElementConstructor() && ! allocator_.allocZeroesMemory()) {
			memset(newData, 0, newSizeBytes);
		}

		if (data_) {
			if (count() > 0) {
				// Copy over the data to the new buffer and free the old one
				if (std::is_trivially_move_constructible<T>::value) {
					memcpy(newData, data_, count() * elementSizeBytes());
				}
				else {
					auto elementsToCopy = count();
					T* src = data_;
					T* dst = newData;

					while (elementsToCopy--) {
						new (dst) T(std::move(*src)); // move-construct element in new array
						src->~T();                    // still need to destroy element after move
						++src;
						++dst;
					}
				}
			}

			allocator_.free(data_);
		}
		
		data_ = newData;
		capacity_ = newCapacity;
	}
	
	
	void resize(uint newCount) {
		auto oldCount = count();
		
		if (newCount > oldCount) {
			if (newCount > capacity()) {
				reserve(newCount);
			}

			// Trivial types are just zeroed, but if there is a user-specified default constructor
			// or if explicit field initialization was used then we need to default-construct
			// new values (in-place).
			if (! canSkipElementConstructor()) {
				static_assert(std::is_default_constructible<T>::value, "T must be default constructible for growing resize");

				auto elementsToConstruct = newCount - oldCount;
				auto newElementPtr = data_ + oldCount;

				// If we were to know that a constructor has no side effects, then we could
				// store a single constructed instance and splat it out over the region.
				while (elementsToConstruct--) {
					new (newElementPtr) T{};
					++newElementPtr;
				}
			}
		}
		else if (newCount < oldCount) {
			// Call destructor for each deleted value for Ts that need it.
			if (! canSkipElementDestructor()) {
				destructRange(data_ + newCount, oldCount - newCount);
			}

			// For trivial Ts we zero the released memory now so append()s don't have to.
			if (canSkipElementConstructor()) {
				auto elementsToClear = oldCount - newCount;
				auto firstDeletedElementPtr = data_ + newCount;
				memset(firstDeletedElementPtr, 0, elementSizeBytes() * elementsToClear);
			}
		}

		count_ = newCount;
	}


	// -- adding elements

	void append(const T& t) {
		if (__builtin_expect(count() == capacity(), 0)) {
			reserve(capacity() * growthFactor_s);
		}

		// copy construct in place
		new (data_ + count_) T{t};
		
		++count_;
	}
	
	
	void prepend(const T& t) {
		if (__builtin_expect(count() == capacity(), 0)) {
			reserve(capacity() * growthFactor_s);
		}

		arrayBlockMove(data_ + 1, data_, count());
		new (data_) T{t};
		
		++count_;
	}


	void emplaceBack() {
		static_assert(canSkipElementConstructor() || std::is_default_constructible<T>::value, "T must be default constructible");

		if (__builtin_expect(count() == capacity(), 0)) {
			reserve(capacity() * growthFactor_s);
		}

		// default-construct in place
		if (! canSkipElementConstructor()) {
			new (data_ + count_) T{};
		}
		
		++count_;
	}

	
	template <typename... Args>
	void emplaceBack(Args&&... args) {
		if (__builtin_expect(count() == capacity(), 0)) {
			reserve(capacity() * growthFactor_s);
		}

		// construct in place
		new (data_ + count_) T{std::forward<Args>(args)...};
		
		++count_;
	}
	
	
	// -- mass append operations
	
	void appendBlock(const T* items, uint itemCount) {
		auto firstPtr = prepareForBlockCopy(itemCount);

		if (std::is_trivially_copy_constructible<T>::value) {
			memcpy(firstPtr, items, itemCount * elementSizeBytes());
		}
		else {
			while (itemCount--) {
				new (firstPtr) T{ *items };
				++firstPtr;
				++items;
			}
		}
	}


	// This method is used when you need the Array to be ready
	// for an external, indirect copy of itemCount items.
	// This ensures that there is enough capacity for the required
	// number of items (using an append()-like growth strategy)
	// and returns a pointer to the first item to copy. The count
	// is already updated.
	T* prepareForBlockCopy(uint itemCount) {
		auto free = capacity() - count();
		auto desiredCapacity = capacity();
		
		while (free < itemCount) {
			desiredCapacity *= 2;
			free = desiredCapacity - count();
		}
		
		reserve(desiredCapacity);
		
		T* firstPtr = data_ + count_;
		count_ += itemCount;
		return firstPtr;
	}


	// -- removing elements
	
	void remove(uint startIndex, uint elementsToRemove = 1) {
		assert(startIndex + elementsToRemove - 1 < count());

		// Call destructor for each deleted value for Ts that need it.
		if (! canSkipElementDestructor()) {
			destructRange(data_ + startIndex, elementsToRemove);
		}

		// Shift back items beyond the deleted range
		int elementsToCopy = count() - startIndex - elementsToRemove;
		if (elementsToCopy > 0) {
			arrayBlockMove(data_ + startIndex, data_ + startIndex + elementsToRemove, elementsToCopy);
		}
		count_ -= elementsToRemove;
		
		// Pre-clear now unused object space
		if (canSkipElementConstructor()) {
			auto elementsToClear = elementsToRemove;
			auto firstDeletedElementPtr = data_ + count();
			memset(firstDeletedElementPtr, 0, elementSizeBytes() * elementsToClear);
		}
	}
	
	
	void popFront() {
		remove(0);
	}
	

	void popBack() {
		remove(count() - 1);
	}


	void clear() {
		if (! canSkipElementDestructor()) {
			destructRange(data_, count());
		}

		// Clear memory for potential newly default-constructed values after the clear()
		if (canSkipElementConstructor()) {
			memset(data_, 0, count() * elementSizeBytes());
		}

		count_ = 0;
	}

	
	// -- subscripting
	
	T& operator[](uint index) {
		assert(index < count());
		return *(data_ + index);
	}

	const T& operator[](uint index) const {
		assert(index < count());
		return *(data_ + index);
	}


	// -- front and back
	
	T& front() {
		assert(count_ > 0);
		return *data_;
	}

	const T& front() const {
		assert(count_ > 0);
		return *data_;
	}

	T& back() {
		assert(count_ > 0);
		return *(data_ + count_ - 1);
	}
	
	const T& back() const {
		assert(count_ > 0);
		return *(data_ + count_ - 1);
	}


	// -- ranges (experimental)
private:
	class Range {
		T *cur_, *end_;
		
	public:
		Range(T* first, T* end)
		: cur_(first - 1)
		, end_(end)
		{}

		bool next() {
			++cur_;
			return cur_ < end_;
		}

		T& current() { return *cur_; }
	};

public:
	Range all() {
		return { data_, data_ + count() };
	}
};


// for compat with stl algorithms and range-for
template <typename T> T* begin(Array<T>& arr) { return arr.elementsBasePtr(); }
template <typename T> const T* begin(const Array<T>& arr) { return arr.elementsBasePtr(); }
template <typename T> T* end(Array<T>& arr) { return arr.elementsBasePtr() + arr.count(); }
template <typename T> const T* end(const Array<T>& arr) { return arr.elementsBasePtr() + arr.count(); }


} // ns container


// -- export to sd namespace
using container::Array;


} // ns stardazed

#endif
