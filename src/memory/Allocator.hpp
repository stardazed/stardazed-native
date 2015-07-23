// ------------------------------------------------------------------
// memory::Allocator - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_MEMORY_ALLOCATOR_H
#define SD_MEMORY_ALLOCATOR_H

#include "system/Config.hpp"
#include <cstring>
#include <cstddef>

namespace stardazed {
namespace memory {


//  ___ _                          _   _    _ _                _
// / __(_)______ ___  __ _ _ _  __| | | |  (_) |_ ___ _ _ __ _| |___
// \__ \ |_ / -_|_-< / _` | ' \/ _` | | |__| |  _/ -_) '_/ _` | (_-<
// |___/_/__\___/__/ \__,_|_||_\__,_| |____|_|\__\___|_| \__,_|_/__/
//

constexpr uint64 kibiBytes(double kb) { return static_cast<uint64>(kb * 1024.); }
constexpr uint64 mibiBytes(double mb) { return static_cast<uint64>(mb * 1024. * 1024.); }
constexpr uint64 gibiBytes(double gb) { return static_cast<uint64>(gb * 1024. * 1024. * 1024.); }
constexpr uint64 tebiBytes(double tb) { return static_cast<uint64>(tb * 1024. * 1024. * 1024. * 1024.); }


inline namespace literals {

	constexpr uint64 operator ""_KB(long double val) {
		return kibiBytes(static_cast<double>(val));
	}

	constexpr uint64 operator ""_KB(unsigned long long val) {
		return kibiBytes(static_cast<double>(val));
	}

	constexpr uint64 operator ""_MB(long double val) {
		return mibiBytes(static_cast<double>(val));
	}

	constexpr uint64 operator ""_MB(unsigned long long val) {
		return mibiBytes(static_cast<double>(val));
	}

	constexpr uint64 operator ""_GB(long double val) {
		return gibiBytes(static_cast<double>(val));
	}

	constexpr uint64 operator ""_GB(unsigned long long val) {
		return gibiBytes(static_cast<double>(val));
	}

	constexpr uint64 operator ""_TB(long double val) {
		return tebiBytes(static_cast<double>(val));
	}

	constexpr uint64 operator ""_TB(unsigned long long val) {
		return tebiBytes(static_cast<double>(val));
	}

} // ins literals


//    _   _ _              _             ___     _            __
//   /_\ | | |___  __ __ _| |_ ___ _ _  |_ _|_ _| |_ ___ _ _ / _|__ _ __ ___
//  / _ \| | / _ \/ _/ _` |  _/ _ \ '_|  | || ' \  _/ -_) '_|  _/ _` / _/ -_)
// /_/ \_\_|_\___/\__\__,_|\__\___/_|   |___|_||_\__\___|_| |_| \__,_\__\___|
//

struct Allocator {
	virtual ~Allocator() = default;
	
	virtual void* alloc(uint64 sizeBytes) = 0;
	virtual void free(void* ptr) = 0;
	
	virtual bool allocZeroesMemory() const = 0;
	virtual uint guaranteedAlignment() const = 0;
};


//  ___         _               _   _ _              _
// / __|_  _ __| |_ ___ _ __   /_\ | | |___  __ __ _| |_ ___ _ _
// \__ \ || (_-<  _/ -_) '  \ / _ \| | / _ \/ _/ _` |  _/ _ \ '_|
// |___/\_, /__/\__\___|_|_|_/_/ \_\_|_\___/\__\__,_|\__\___/_|
//      |__/

// Allocates memory on the heap using the default system allocator.

struct SystemAllocator final : Allocator {
	void* alloc(uint64 sizeBytes) final {
		return calloc(1, sizeBytes);
	}

	void free(void* ptr) final {
		free(ptr);
	}
	
	bool allocZeroesMemory() const final { return true; }
	uint guaranteedAlignment() const final { return alignof(max_align_t); }
	
	static SystemAllocator& sharedInstance();

private:
	SystemAllocator() = default; // singleton
};


//  ___ _           _      _   _ _              _
// / __| |_ __ _ __| |__  /_\ | | |___  __ __ _| |_ ___ _ _
// \__ \  _/ _` / _| / / / _ \| | / _ \/ _/ _` |  _/ _ \ '_|
// |___/\__\__,_\__|_\_\/_/ \_\_|_\___/\__\__,_|\__\___/_|
//

// Allocates directly on the stack and thus free() is a nop.
// Use only for small(ish), local allocations.

struct StackAllocator final : Allocator {
	void* alloc(uint64 sizeBytes) final {
		// FIXME: this call varies per platform
		return alloca(sizeBytes);
	}
	
	void free(void* /* ptr */) final {
		/* nothing, exiting the current function will free */
	}
	
	bool allocZeroesMemory() const final { return false; }
	uint guaranteedAlignment() const final {
		return 1; // FIXME: this varies per platform
	}
	
	static StackAllocator& sharedInstance();
	
private:
	StackAllocator() = default; // singleton
};


} // ns memory
} // ns stardazed

#endif
