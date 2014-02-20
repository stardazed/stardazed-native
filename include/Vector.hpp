// ------------------------------------------------------------------
// math::Vector, revision the 3rd - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_MATH_VECTOR_H
#define SD_MATH_VECTOR_H

#include <cassert>
#include <cmath>

namespace stardazed {
namespace math {


#define VEC_SUBSCRIPT_OP(size) \
constexpr T& operator[](const size_t index) { \
	assert(index < size); \
	return data[index]; \
} \
constexpr T operator[](const size_t index) const { \
	assert(index < size); \
	return data[index]; \
}


template <size_t N, typename T = float>
struct Vector {
	T data[N];

	VEC_SUBSCRIPT_OP(N)
};

using Vec2 = Vector<2>;
using Vec3 = Vector<3>;
using Vec4 = Vector<4>;


template <typename T>
struct Vector<2, T> {
	union {
		T data[2];
		struct { T x, y; };
		struct { T u, v; };
	};
	
	explicit constexpr Vector(const T fill) : x(fill), y(fill) {}
	constexpr Vector(const T x, const T y) : x(x), y(y) {}
	constexpr Vector() : Vector(T(0)) {}
	
	VEC_SUBSCRIPT_OP(2)
};


template <typename T>
struct Vector<3, T> {
	union {
		T data[3];
		struct { T x, y, z; };
		struct { T r, g, b; };
		Vector<2, T> xy;
		Vector<2, T> uv;
	};
	
	explicit constexpr Vector(const T fill) : x(fill), y(fill), z(fill) {}
	constexpr Vector(const T x, const T y, const T z) : x(x), y(y), z(z) {}
	constexpr Vector(const Vector<2, T>& xy, const T z) : x(xy.x), y(xy.y), z(z) {}
	constexpr Vector() : Vector(T(0)) {}

	VEC_SUBSCRIPT_OP(3)
};


template <typename T>
struct Vector<4, T> {
	union {
		T data[4];
		struct { T x, y, z, w; };
		struct { T r, g, b, a; };
		Vector<2, T> xy;
		Vector<2, T> uv;
		Vector<3, T> xyz;
		Vector<3, T> rgb;
	};

	VEC_SUBSCRIPT_OP(4)
};

#undef VEC_SUBSCRIPT_OP

	
} // ns math
} // ns stardazed

#endif
