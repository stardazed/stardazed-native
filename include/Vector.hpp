// ------------------------------------------------------------------
// Vector - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_MATH_VECTOR_H
#define SD_MATH_VECTOR_H

#include <array>
#include <iostream>
#include <type_traits>

namespace stardazed {
namespace math {


namespace /* detail */ {
	template <typename T>
	using TVec3 = std::array<T, 3>;

	template <typename T>
	using TVec4 = std::array<T, 4>;
}


using Vec3  = TVec3<float>;
using DVec3 = TVec3<double>;
using IVec3 = TVec3<int>;

using Vec4  = TVec4<float>;
using DVec4 = TVec4<double>;
using IVec4 = TVec4<int>;



#define TVEC3_TVEC3_OPERATOR(op) \
template <typename T> \
constexpr TVec3<T> operator op (const TVec3<T>& lhs, const TVec3<T>& rhs) { \
	return { lhs[0] op rhs[0], lhs[1] op rhs[1], lhs[2] op rhs[2] }; \
}

TVEC3_TVEC3_OPERATOR(+)
TVEC3_TVEC3_OPERATOR(-)
TVEC3_TVEC3_OPERATOR(*)
TVEC3_TVEC3_OPERATOR(/)

#undef TVEC3_TVEC3_OPERATOR



#define TVEC3_SCALAR_OPERATOR(op) \
template <typename T, typename S> \
constexpr \
std::enable_if_t<std::is_convertible<S, T>::value, TVec3<T>> \
operator op (const TVec3<T>& lhs, const S rhs) { \
	return { lhs[0] op rhs, lhs[1] op rhs, lhs[2] op rhs }; \
}

TVEC3_SCALAR_OPERATOR(+)
TVEC3_SCALAR_OPERATOR(-)
TVEC3_SCALAR_OPERATOR(*)
TVEC3_SCALAR_OPERATOR(/)

#undef TVEC3_SCALAR_OPERATOR



#define SCALAR_TVEC3_OPERATOR(op) \
template <typename T, typename S> \
constexpr \
std::enable_if_t<std::is_convertible<S, T>::value, TVec3<T>> \
operator op (const S lhs, const TVec3<T>& rhs) { \
	return { lhs op rhs[0], lhs op rhs[1], lhs op rhs[2] }; \
}

SCALAR_TVEC3_OPERATOR(+)
SCALAR_TVEC3_OPERATOR(-)
SCALAR_TVEC3_OPERATOR(*)
SCALAR_TVEC3_OPERATOR(/)

#undef SCALAR_TVEC3_OPERATOR



#define TVEC3_TVEC3_ASSIGN_OPERATOR(op) \
template <typename T> \
TVec3<T>& operator op(TVec3<T>& lhs, const TVec3<T>& rhs) { \
	lhs[0] op rhs[0]; lhs[1] op rhs[1]; lhs[2] op rhs[2]; \
	return lhs; \
}

TVEC3_TVEC3_ASSIGN_OPERATOR(+=)
TVEC3_TVEC3_ASSIGN_OPERATOR(-=)
TVEC3_TVEC3_ASSIGN_OPERATOR(*=)
TVEC3_TVEC3_ASSIGN_OPERATOR(/=)

#undef TVEC3_TVEC3_ASSIGN_OPERATOR



#define TVEC3_SCALAR_ASSIGN_OPERATOR(op) \
template <typename T, typename S> \
TVec3<T>& operator op(TVec3<T>& lhs, const S rhs) { \
	lhs[0] op rhs; lhs[1] op rhs; lhs[2] op rhs; \
	return lhs; \
}

TVEC3_SCALAR_ASSIGN_OPERATOR(+=)
TVEC3_SCALAR_ASSIGN_OPERATOR(-=)
TVEC3_SCALAR_ASSIGN_OPERATOR(*=)
TVEC3_SCALAR_ASSIGN_OPERATOR(/=)

#undef TVEC3_SCALAR_ASSIGN_OPERATOR



#define TVEC4_TVEC4_OPERATOR(op) \
template <typename T> \
constexpr TVec4<T> operator op (const TVec4<T>& lhs, const TVec4<T>& rhs) { \
	return { lhs[0] op rhs[0], lhs[1] op rhs[1], lhs[2] op rhs[2], lhs[3] op rhs[3] }; \
}

TVEC4_TVEC4_OPERATOR(+)
TVEC4_TVEC4_OPERATOR(-)
TVEC4_TVEC4_OPERATOR(*)
TVEC4_TVEC4_OPERATOR(/)

#undef TVEC4_TVEC4_OPERATOR



#define TVEC4_SCALAR_OPERATOR(op) \
template <typename T, typename S> \
constexpr \
std::enable_if_t<std::is_convertible<S, T>::value, TVec4<T>> \
operator op (const TVec4<T>& lhs, const S rhs) { \
	return { lhs[0] op rhs, lhs[1] op rhs, lhs[2] op rhs, lhs[3] op rhs }; \
}

TVEC4_SCALAR_OPERATOR(+)
TVEC4_SCALAR_OPERATOR(-)
TVEC4_SCALAR_OPERATOR(*)
TVEC4_SCALAR_OPERATOR(/)

#undef TVEC4_SCALAR_OPERATOR



#define SCALAR_TVEC4_OPERATOR(op) \
template <typename T, typename S> \
constexpr \
std::enable_if_t<std::is_convertible<S, T>::value, TVec4<T>> \
operator op (const S lhs, const TVec4<T>& rhs) { \
	return { lhs op rhs[0], lhs op rhs[1], lhs op rhs[2], lhs op rhs[3] }; \
}

SCALAR_TVEC4_OPERATOR(+)
SCALAR_TVEC4_OPERATOR(-)
SCALAR_TVEC4_OPERATOR(*)
SCALAR_TVEC4_OPERATOR(/)

#undef SCALAR_TVEC4_OPERATOR



#define TVEC4_TVEC4_ASSIGN_OPERATOR(op) \
template <typename T> \
TVec4<T>& operator op (TVec4<T>& lhs, const TVec4<T>& rhs) { \
	lhs[0] op rhs[0]; lhs[1] op rhs[1]; lhs[2] op rhs[2]; lhs[3] op rhs[3]; \
	return lhs; \
}

TVEC4_TVEC4_ASSIGN_OPERATOR(+=)
TVEC4_TVEC4_ASSIGN_OPERATOR(-=)
TVEC4_TVEC4_ASSIGN_OPERATOR(*=)
TVEC4_TVEC4_ASSIGN_OPERATOR(/=)

#undef TVEC4_TVEC4_ASSIGN_OPERATOR



#define TVEC4_SCALAR_ASSIGN_OPERATOR(op) \
template <typename T, typename S> \
std::enable_if_t<std::is_convertible<S, T>::value, TVec4<T>&> \
operator op (TVec4<T>& lhs, const S rhs) { \
	lhs[0] op rhs; lhs[1] op rhs; lhs[2] op rhs; lhs[3] op rhs; \
	return lhs; \
}

TVEC4_SCALAR_ASSIGN_OPERATOR(+=)
TVEC4_SCALAR_ASSIGN_OPERATOR(-=)
TVEC4_SCALAR_ASSIGN_OPERATOR(*=)
TVEC4_SCALAR_ASSIGN_OPERATOR(/=)
	
#undef TVEC4_SCALAR_ASSIGN_OPERATOR


// ---- easily print vecs
template <typename T>
std::ostream& operator<< (std::ostream& os, const TVec3<T>& vec) {
	return os << '(' << vec[0] << ',' << vec[1] << ',' << vec[2] << ')';
}

template <typename T>
std::ostream& operator<< (std::ostream& os, const TVec4<T>& vec) {
	return os << '(' << vec[0] << ',' << vec[1] << ',' << vec[2] << ',' << vec[3] << ')';
}


} // ns math
} // ns stardazed

#endif
