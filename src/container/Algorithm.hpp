// ------------------------------------------------------------------
// container::Algorithm - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_CONTAINER_ALGORITHM_H
#define SD_CONTAINER_ALGORITHM_H

#include "system/Config.hpp"

#include <type_traits>

namespace stardazed {
namespace container {


template <typename T, bool Trivial = std::is_trivially_copy_constructible<T>::value>
void arrayBlockMove(T*, const T*, uint count);


template <typename T>
void arrayBlockMove<true>(T* dest, const T* src, uint count) {
	
}


} // ns container
} // ns stardazed

#endif
