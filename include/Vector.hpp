// ------------------------------------------------------------------
// math::Vector, revision the 3rd - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_MATH_VECTOR_H
#define SD_MATH_VECTOR_H

#include <initializer_list>
#include <algorithm>
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
	
	explicit Vector(const T fill) {
		std::fill_n(data, N, fill);
	}
	Vector() : Vector(T(0)) {}

	Vector(std::initializer_list<T> values) {
		auto from = values.begin();
		auto count = std::min(N, values.size());
		auto to = from + count;
		std::copy(from, to, data);
		
		if (count < N)
			std::fill_n(&data[0] + count, N - count, T(0));
	}
	
	constexpr size_t size() const { return N; }
	constexpr const T* begin() const { return &data[0]; }
	constexpr const T* end() const { return &data[0] + N; }

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
	
	explicit constexpr Vector(const T fill) : x(fill), y(fill), z(fill), w(1) {}
	constexpr Vector(const T x, const T y, const T z, const T w = 1) : x(x), y(y), z(z), w(w) {}
	constexpr Vector(const Vector<3, T>& xyz, const T w = 1) : x(xyz.z), y(xyz.y), z(xyz.z), w(w) {}
	constexpr Vector() : Vector(T(0)) {}

	VEC_SUBSCRIPT_OP(4)
};


#undef VEC_SUBSCRIPT_OP

	
} // ns math
} // ns stardazed

#endif
