// ------------------------------------------------------------------
// Matrix - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_MATH_MATRIX_H
#define SD_MATH_MATRIX_H

#include <array>
#include <type_traits>

namespace stardazed {
namespace math {


namespace /* detail */ {
	template <typename T>
	using TMatrix3 = std::array<T, 9>;

	template <typename T>
	using TMatrix4 = std::array<T, 16>;
}


using Matrix3 = TMatrix3<float>;
using DMatrix3 = TMatrix3<double>;

using Matrix4 = TMatrix4<float>;
using DMatrix4 = TMatrix4<double>;



#define TMATRIX3_TMATRIX3_OPERATOR(op) \
template <typename T> \
constexpr TMatrix3<T> operator op (const TMatrix3<T>& lhs, const TMatrix3<T>& rhs) { \
	return { \
		lhs[0] op rhs[0], lhs[1] op rhs[1], lhs[2] op rhs[2], \
		lhs[3] op rhs[3], lhs[4] op rhs[4], lhs[5] op rhs[5], \
		lhs[6] op rhs[6], lhs[7] op rhs[7], lhs[8] op rhs[8]  \
	}; \
}

TMATRIX3_TMATRIX3_OPERATOR(+)
TMATRIX3_TMATRIX3_OPERATOR(-)
TMATRIX3_TMATRIX3_OPERATOR(*)
TMATRIX3_TMATRIX3_OPERATOR(/)



#define TMATRIX3_SCALAR_OPERATOR(op) \
template <typename T, typename S> \
constexpr \
std::enable_if_t<std::is_convertible<S, T>::value, TMatrix3<T>> \
operator op (const TMatrix3<T>& lhs, const S rhs) { \
	return { \
		lhs[0] op rhs, lhs[1] op rhs, lhs[2] op rhs, \
		lhs[3] op rhs, lhs[4] op rhs, lhs[5] op rhs, \
		lhs[6] op rhs, lhs[7] op rhs, lhs[8] op rhs  \
	}; \
}

TMATRIX3_SCALAR_OPERATOR(+)
TMATRIX3_SCALAR_OPERATOR(-)
TMATRIX3_SCALAR_OPERATOR(*)
TMATRIX3_SCALAR_OPERATOR(/)



#define TMATRIX3_TMATRIX3_ASSIGN_OP(op) \
template <typename T> \
TMatrix3<T>& operator op (TMatrix3<T>& lhs, const TMatrix3<T>& rhs) { \
	lhs[0] op rhs[0]; lhs[1] op rhs[1]; lhs[2] op rhs[2]; \
	lhs[3] op rhs[3]; lhs[4] op rhs[4]; lhs[5] op rhs[5]; \
	lhs[6] op rhs[6]; lhs[7] op rhs[7]; lhs[8] op rhs[8]; \
	return lhs; \
}

TMATRIX3_TMATRIX3_ASSIGN_OP(+=)
TMATRIX3_TMATRIX3_ASSIGN_OP(-=)
TMATRIX3_TMATRIX3_ASSIGN_OP(*=)
TMATRIX3_TMATRIX3_ASSIGN_OP(/=)



#define TMATRIX3_SCALAR_ASSIGN_OP(op) \
template <typename T, typename S> \
std::enable_if_t<std::is_convertible<S, T>::value, TMatrix3<T>&> \
operator op (TMatrix3<T>& lhs, const S rhs) { \
	lhs[0] += rhs; lhs[1] += rhs; lhs[2] += rhs; \
	lhs[3] += rhs; lhs[4] += rhs; lhs[5] += rhs; \
	lhs[6] += rhs; lhs[7] += rhs; lhs[8] += rhs; \
	return lhs; \
}

TMATRIX3_SCALAR_ASSIGN_OP(+=)
TMATRIX3_SCALAR_ASSIGN_OP(-=)
TMATRIX3_SCALAR_ASSIGN_OP(*=)
TMATRIX3_SCALAR_ASSIGN_OP(/=)


} // ns math
} // ns stardazed

#endif
