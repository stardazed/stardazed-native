// ------------------------------------------------------------------
// math::Constants - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_MATH_CONSTANTS_H
#define SD_MATH_CONSTANTS_H


namespace stardazed {
namespace math {


template <typename T>
constexpr const T Pi = T{ 3.14159265358979323846 };

template <typename T>
constexpr const T Tau = Pi<T> * T{2};


} // ns math
} // ns stardazed


#endif
