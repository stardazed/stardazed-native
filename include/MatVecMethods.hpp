// ------------------------------------------------------------------
// math::MatVecMethods - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_MATH_MATVECMETHODS_H
#define SD_MATH_MATVECMETHODS_H

#include "Vector.hpp"
#include "Matrix.hpp"
#include <cmath>


namespace stardazed {
namespace math {


// __     __        _               __  __      _   _               _
// \ \   / /__  ___| |_ ___  _ __  |  \/  | ___| |_| |__   ___   __| |___
//  \ \ / / _ \/ __| __/ _ \| '__| | |\/| |/ _ \ __| '_ \ / _ \ / _` / __|
//   \ V /  __/ (__| || (_) | |    | |  | |  __/ |_| | | | (_) | (_| \__ \
//    \_/ \___|\___|\__\___/|_|    |_|  |_|\___|\__|_| |_|\___/ \__,_|___/

template <typename T>
T length(const TVec3<T>& vec) {
	return std::sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
}


template <typename T>
T length(const TVec4<T>& vec) {
	return std::sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2] + vec[3] * vec[3]);
}


template <typename T>
TVec3<T> normalize(const TVec3<T>& vec) {
	return vec / length(vec);
}


template <typename T>
TVec4<T> normalize(const TVec4<T>& vec) {
	return vec / length(vec);
}


template <typename T>
void normalizeInPlace(TVec3<T>& vec) {
	vec /= length(vec);
}


template <typename T>
void normalizeInPlace(TVec4<T>& vec) {
	vec /= length(vec);
}


template <typename T>
T dot(const TVec3<T>& a, const TVec3<T>& b) {
	return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}


template <typename T>
T dot(const TVec4<T>& a, const TVec4<T>& b) {
	// w component is ignored
	return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}


template <typename T>
TVec3<T> cross(const TVec3<T>& a, const TVec3<T>& b) {
	TVec3<T> v;
	v[0] = a[1] * b[2] - a[2] * b[1];
	v[1] = a[2] * b[0] - a[0] * b[2];
	v[2] = a[0] * b[1] - a[1] * b[0];

	return v;
}


template <typename T>
TVec4<T> cross(const TVec4<T>& a, const TVec4<T>& b) {
	TVec4<T> v;
	v[0] = a[1] * b[2] - a[2] * b[1];
	v[1] = a[2] * b[0] - a[0] * b[2];
	v[2] = a[0] * b[1] - a[1] * b[0];
	v[3] = static_cast<T>(1.0);

	return v;
}



//  __  __       _        _        __  __      _   _               _
// |  \/  | __ _| |_ _ __(_)_  __ |  \/  | ___| |_| |__   ___   __| |___
// | |\/| |/ _` | __| '__| \ \/ / | |\/| |/ _ \ __| '_ \ / _ \ / _` / __|
// | |  | | (_| | |_| |  | |>  <  | |  | |  __/ |_| | | | (_) | (_| \__ \
// |_|  |_|\__,_|\__|_|  |_/_/\_\ |_|  |_|\___|\__|_| |_|\___/ \__,_|___/

template <typename T>
constexpr TMatrix3<T> unityMatrix3() {
	return {
		1, 0, 0,
		0, 1, 0,
		0, 0, 1
	};
}


template <typename T>
constexpr TMatrix4<T> unityMatrix4() {
	return {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
}


template <typename T>
constexpr TMatrix4<T> translateMatrix4(T x, T y, T z) {
	auto mat = unityMatrix4<T>();
	mat[12] = x;
	mat[13] = y;
	mat[14] = z;
	return mat;
}



} // ns math
} // ns stardazed

#endif
