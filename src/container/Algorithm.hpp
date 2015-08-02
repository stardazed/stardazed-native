// ------------------------------------------------------------------
// container::Algorithm - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_CONTAINER_ALGORITHM_H
#define SD_CONTAINER_ALGORITHM_H

#include "system/Config.hpp"

#include <cstring>
#include <cstdlib>
#include <type_traits>

namespace stardazed {
namespace container {


template <typename T>
void arrayBlockMove(T* dest, const T* src, uint count) {
	if (std::is_trivially_copy_assignable<T>::value) {
		memmove(dest, src, count * sizeof(T));
	}
	else {
		auto absDistBytes = std::abs(ptrdiff_t(dest) - ptrdiff_t(src));
		bool noOverlap = absDistBytes >= ptrdiff_t(count * sizeof(T));

		if (noOverlap || dest < src) {
			while (count--)
				*dest++ = *src++;
		}
		else {
			auto srcEnd = src + count - 1;
			auto destEnd = dest + count - 1;
			
			while (count--)
				*destEnd-- = *srcEnd--;
		}
	}
}


template <typename C, typename F>
void forEach(C& cont, F&& fn) {
	auto all = cont.all();
	
	while (all.next()) {
		fn(all.current());
	}
}


} // ns container
} // ns stardazed

#endif
