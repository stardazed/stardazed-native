// ------------------------------------------------------------------
// math::Matrix - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_MATH_MATRIX_H
#define SD_MATH_MATRIX_H

#include "Vector.hpp"
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


//  __  __       _        _      _____    ___
// |  \/  | __ _| |_ _ __(_)_  _|___ /   / _ \ _ __  ___
// | |\/| |/ _` | __| '__| \ \/ / |_ \  | | | | '_ \/ __|
// | |  | | (_| | |_| |  | |>  < ___) | | |_| | |_) \__ \
// |_|  |_|\__,_|\__|_|  |_/_/\_\____/   \___/| .__/|___/
//                                            |_|

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

#undef TMATRIX3_TMATRIX3_OPERATOR



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

#undef TMATRIX3_SCALAR_OPERATOR



#define SCALAR_TMATRIX3_OPERATOR(op) \
template <typename T, typename S> \
constexpr \
std::enable_if_t<std::is_convertible<S, T>::value, TMatrix3<T>> \
operator op (const S lhs, const TMatrix3<T>& rhs) { \
	return { \
		lhs op rhs[0], lhs op rhs[1], lhs op rhs[2], \
		lhs op rhs[3], lhs op rhs[4], lhs op rhs[5], \
		lhs op rhs[6], lhs op rhs[7], lhs op rhs[8]  \
	}; \
}
	
SCALAR_TMATRIX3_OPERATOR(+)
SCALAR_TMATRIX3_OPERATOR(-)
SCALAR_TMATRIX3_OPERATOR(*)
SCALAR_TMATRIX3_OPERATOR(/)

#undef SCALAR_TMATRIX3_OPERATOR
	
	
	
#define TMATRIX3_TMATRIX3_ASSIGN_OPERATOR(op) \
template <typename T> \
TMatrix3<T>& operator op (TMatrix3<T>& lhs, const TMatrix3<T>& rhs) { \
	lhs[0] op rhs[0]; lhs[1] op rhs[1]; lhs[2] op rhs[2]; \
	lhs[3] op rhs[3]; lhs[4] op rhs[4]; lhs[5] op rhs[5]; \
	lhs[6] op rhs[6]; lhs[7] op rhs[7]; lhs[8] op rhs[8]; \
	return lhs; \
}

TMATRIX3_TMATRIX3_ASSIGN_OPERATOR(+=)
TMATRIX3_TMATRIX3_ASSIGN_OPERATOR(-=)
TMATRIX3_TMATRIX3_ASSIGN_OPERATOR(*=)
TMATRIX3_TMATRIX3_ASSIGN_OPERATOR(/=)

#undef TMATRIX3_TMATRIX3_ASSIGN_OPERATOR



#define TMATRIX3_SCALAR_ASSIGN_OPERATOR(op) \
template <typename T, typename S> \
std::enable_if_t<std::is_convertible<S, T>::value, TMatrix3<T>&> \
operator op (TMatrix3<T>& lhs, const S rhs) { \
	lhs[0] += rhs; lhs[1] += rhs; lhs[2] += rhs; \
	lhs[3] += rhs; lhs[4] += rhs; lhs[5] += rhs; \
	lhs[6] += rhs; lhs[7] += rhs; lhs[8] += rhs; \
	return lhs; \
}

TMATRIX3_SCALAR_ASSIGN_OPERATOR(+=)
TMATRIX3_SCALAR_ASSIGN_OPERATOR(-=)
TMATRIX3_SCALAR_ASSIGN_OPERATOR(*=)
TMATRIX3_SCALAR_ASSIGN_OPERATOR(/=)

#undef TMATRIX3_SCALAR_ASSIGN_OPERATOR


template <typename T>
TVec3<T> operator * (const TMatrix3<T>& mat, const TVec3<T>& vec) {
	T x = vec[0], y = vec[1], z = vec[2];

	return {
		(mat[0] * x) + (mat[1] * y) + (mat[2] * z),
		(mat[3] * x) + (mat[4] * y) + (mat[5] * z),
		(mat[6] * x) + (mat[7] * y) + (mat[8] * z)
	};
}


//  __  __       _        _      _  _      ___
// |  \/  | __ _| |_ _ __(_)_  _| || |    / _ \ _ __  ___
// | |\/| |/ _` | __| '__| \ \/ / || |_  | | | | '_ \/ __|
// | |  | | (_| | |_| |  | |>  <|__   _| | |_| | |_) \__ \
// |_|  |_|\__,_|\__|_|  |_/_/\_\  |_|    \___/| .__/|___/
//                                             |_|

#define TMATRIX4_TMATRIX4_OPERATOR(op) \
template <typename T> \
constexpr TMatrix4<T> operator op (const TMatrix4<T>& lhs, const TMatrix4<T>& rhs) { \
	return { \
		lhs[0]  op rhs[0],  lhs[1]  op rhs[1],  lhs[2]  op rhs[2],  lhs[3]  op rhs[3],  \
		lhs[4]  op rhs[4],  lhs[5]  op rhs[5],  lhs[6]  op rhs[6],  lhs[7]  op rhs[7],  \
		lhs[8]  op rhs[8],  lhs[9]  op rhs[9],  lhs[10] op rhs[10], lhs[11] op rhs[11], \
		lhs[12] op rhs[12], lhs[13] op rhs[13], lhs[14] op rhs[14], lhs[15] op rhs[15]  \
	}; \
}
	
TMATRIX4_TMATRIX4_OPERATOR(+)
TMATRIX4_TMATRIX4_OPERATOR(-)
	
#undef TMATRIX4_TMATRIX4_OPERATOR
	
	
	
#define TMATRIX4_SCALAR_OPERATOR(op) \
template <typename T, typename S> \
constexpr \
std::enable_if_t<std::is_convertible<S, T>::value, TMatrix4<T>> \
operator op (const TMatrix4<T>& lhs, const S rhs) { \
	return { \
		lhs[0]  op rhs, lhs[1]  op rhs, lhs[2]  op rhs, lhs[3]  op rhs, \
		lhs[4]  op rhs, lhs[5]  op rhs, lhs[6]  op rhs, lhs[7]  op rhs, \
		lhs[8]  op rhs, lhs[9]  op rhs, lhs[10] op rhs, lhs[11] op rhs, \
		lhs[12] op rhs, lhs[13] op rhs, lhs[14] op rhs, lhs[15] op rhs  \
	}; \
}
	
TMATRIX4_SCALAR_OPERATOR(+)
TMATRIX4_SCALAR_OPERATOR(-)
TMATRIX4_SCALAR_OPERATOR(*)
TMATRIX4_SCALAR_OPERATOR(/)
	
#undef TMATRIX4_SCALAR_OPERATOR
	
	
	
#define SCALAR_TMATRIX4_OPERATOR(op) \
template <typename T, typename S> \
constexpr \
std::enable_if_t<std::is_convertible<S, T>::value, TMatrix4<T>> \
operator op (const S lhs, const TMatrix4<T>& rhs) { \
	return { \
		lhs op rhs[0],  lhs op rhs[1],  lhs op rhs[2],  lhs op rhs[3],  \
		lhs op rhs[4],  lhs op rhs[5],  lhs op rhs[6],  lhs op rhs[7],  \
		lhs op rhs[8],  lhs op rhs[9],  lhs op rhs[10], lhs op rhs[11], \
		lhs op rhs[12], lhs op rhs[13], lhs op rhs[14], lhs op rhs[15]  \
	}; \
}
	
SCALAR_TMATRIX4_OPERATOR(+)
SCALAR_TMATRIX4_OPERATOR(-)
SCALAR_TMATRIX4_OPERATOR(*)
SCALAR_TMATRIX4_OPERATOR(/)
	
#undef SCALAR_TMATRIX4_OPERATOR
	
	
	
#define TMATRIX4_TMATRIX4_ASSIGN_OPERATOR(op) \
template <typename T> \
TMatrix4<T>& operator op (TMatrix4<T>& lhs, const TMatrix4<T>& rhs) { \
	lhs op rhs[0];  lhs op rhs[1];  lhs op rhs[2];  lhs op rhs[3];  \
	lhs op rhs[4];  lhs op rhs[5];  lhs op rhs[6];  lhs op rhs[7];  \
	lhs op rhs[8];  lhs op rhs[9];  lhs op rhs[10]; lhs op rhs[11]; \
	lhs op rhs[12]; lhs op rhs[13]; lhs op rhs[14]; lhs op rhs[15]; \
	return lhs; \
}

TMATRIX4_TMATRIX4_ASSIGN_OPERATOR(+=)
TMATRIX4_TMATRIX4_ASSIGN_OPERATOR(-=)
	
#undef TMATRIX4_TMATRIX4_ASSIGN_OPERATOR
	
	
	
#define TMATRIX4_SCALAR_ASSIGN_OPERATOR(op) \
template <typename T, typename S> \
std::enable_if_t<std::is_convertible<S, T>::value, TMatrix4<T>&> \
operator op (TMatrix4<T>& lhs, const S rhs) { \
	lhs op rhs[0];  lhs op rhs[1];  lhs op rhs[2];  lhs op rhs[3];  \
	lhs op rhs[4];  lhs op rhs[5];  lhs op rhs[6];  lhs op rhs[7];  \
	lhs op rhs[8];  lhs op rhs[9];  lhs op rhs[10]; lhs op rhs[11]; \
	lhs op rhs[12]; lhs op rhs[13]; lhs op rhs[14]; lhs op rhs[15]; \
	return lhs; \
}

TMATRIX4_SCALAR_ASSIGN_OPERATOR(+=)
TMATRIX4_SCALAR_ASSIGN_OPERATOR(-=)
TMATRIX4_SCALAR_ASSIGN_OPERATOR(*=)
TMATRIX4_SCALAR_ASSIGN_OPERATOR(/=)

#undef TMATRIX4_SCALAR_ASSIGN_OPERATOR


template <typename T>
TVec4<T> operator * (const TMatrix4<T>& mat, const TVec4<T>& vec) {
	T x = vec[0], y = vec[1], z = vec[2], w = vec[3];
	
	return {
		(mat[0]  * x) + (mat[1]  * y) + (mat[2]  * z) + (mat[3]  * w),
		(mat[4]  * x) + (mat[5]  * y) + (mat[6]  * z) + (mat[7]  * w),
		(mat[8]  * x) + (mat[9]  * y) + (mat[10] * z) + (mat[11] * w),
		(mat[12] * x) + (mat[13] * y) + (mat[14] * z) + (mat[15] * w)
	};
}


} // ns math
} // ns stardazed

#endif
