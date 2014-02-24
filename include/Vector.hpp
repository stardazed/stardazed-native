// ------------------------------------------------------------------
// math::Vector, revision the 3rd - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_MATH_VECTOR_H
#define SD_MATH_VECTOR_H

#include <initializer_list>
#include <algorithm>
#include <functional>
#include <cassert>
#include <cmath>

namespace stardazed {
namespace math {

	
template <typename Derived, size_t N, typename T>
struct VectorBase {
	constexpr T& operator[](const size_t index) {
		assert(index < N);
		return static_cast<Derived*>(this)->data[index];
	}

	constexpr T operator[](const size_t index) const {
		assert(index < N);
		return static_cast<const Derived*>(this)->data[index];
	}

	constexpr size_t size() const { return N; }
	constexpr T* begin() { return &static_cast<Derived*>(this)->data[0]; }
	constexpr T* end() { return &static_cast<Derived*>(this)->data[0] + N; }
	constexpr const T* begin() const { return &static_cast<const Derived*>(this)->data[0]; }
	constexpr const T* end() const { return &static_cast<const Derived*>(this)->data[0] + N; }
};


template <size_t N, typename T = float>
struct Vector : public VectorBase<Vector<N, T>, N, T> {
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
};

using Vec2 = Vector<2>;
using Vec3 = Vector<3>;
using Vec4 = Vector<4>;


template <typename T>
struct Vector<2, T> : public VectorBase<Vector<2, T>, 2, T> {
	union {
		T data[2];
		struct { T x, y; };
		struct { T u, v; };
	};
	
	explicit constexpr Vector(const T fill) : x(fill), y(fill) {}
	constexpr Vector(const T x, const T y) : x(x), y(y) {}
	constexpr Vector() : Vector(T(0)) {}
};


template <typename T>
struct Vector<3, T> : public VectorBase<Vector<3, T>, 3, T> {
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
};


template <typename T>
struct Vector<4, T> : public VectorBase<Vector<4, T>, 4, T> {
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
	explicit constexpr Vector(const Vector<3, T>& xyz, const T w = 1) : x(xyz.z), y(xyz.y), z(xyz.z), w(w) {}
	constexpr Vector() : Vector(T(0)) {}
};



namespace detail {
	template <typename Op, size_t N, typename T>
	Vector<N, T>& componentWiseAssignOperator(Vector<N, T>& a, const Vector<N, T>& b) {
		auto op = Op();
		std::transform(a.begin(), a.end(), b.begin(), a.begin(), [&op](auto va, auto vb) {
			return op(va, vb);
		});
		return a;
	}
	
	template <typename Op, typename T>
	Vector<3, T>& componentWiseAssignOperator(Vector<3, T>& a, const Vector<3, T>& b) {
		auto op = Op();
		a[0] = op(a[0], b[0]);
		a[1] = op(a[1], b[1]);
		a[2] = op(a[2], b[2]);
		return a;
	}
	
	template <typename Op, size_t N, typename T>
	Vector<N, T> componentWiseOperator(const Vector<N, T>& a, const Vector<N, T>& b) {
		Vector<N, T> result = a;
		componentWiseAssignOperator<Op>(result, b);
		return result;
	}

	template <typename Op, size_t N, typename T>
	Vector<N, T>& scalarAssignOperator(Vector<N, T>& vec, const T scalar) {
		auto op = Op();
		std::transform(vec.begin(), vec.end(), vec.begin(), [&op, scalar](auto va) {
			return op(va, scalar);
		});
		return vec;
	}
	
	template <typename Op, size_t N, typename T>
	Vector<N, T> scalarOperator(const Vector<N, T>& vec, const T scalar) {
		Vector<N, T> result = vec;
		scalarAssignOperator<Op>(result, scalar);
		return result;
	}
}


// ---- Addition

template <size_t N, typename T>
Vector<N, T> operator +(const Vector<N, T>& a, const Vector<N, T>& b) {
	return detail::componentWiseOperator<std::plus<T>>(a, b);
}


template <size_t N, typename T>
Vector<N, T>& operator +=(Vector<N, T>& a, const Vector<N, T>& b) {
	return detail::componentWiseAssignOperator<std::plus<T>>(a, b);
}


template <size_t N, typename T>
Vector<N, T> operator +(const Vector<N, T>& vec, const T scalar) {
	return detail::scalarOperator<std::plus<T>>(vec, scalar);
}


template <size_t N, typename T>
Vector<N, T>& operator +=(Vector<N, T>& vec, const T scalar) {
	return detail::scalarAssignOperator<std::plus<T>>(vec, scalar);
}


// ---- Subtraction

template <size_t N, typename T>
Vector<N, T> operator -(const Vector<N, T>& a, const Vector<N, T>& b) {
	return detail::componentWiseOperator<std::minus<T>>(a, b);
}


template <size_t N, typename T>
Vector<N, T>& operator -=(Vector<N, T>& a, const Vector<N, T>& b) {
	return detail::componentWiseAssignOperator<std::minus<T>>(a, b);
}


template <size_t N, typename T>
Vector<N, T> operator -(const Vector<N, T>& vec, const T scalar) {
	return detail::scalarOperator<std::minus<T>>(vec, scalar);
}


template <size_t N, typename T>
Vector<N, T>& operator -=(Vector<N, T>& vec, const T scalar) {
	return detail::scalarAssignOperator<std::minus<T>>(vec, scalar);
}

	
// ---- Multiplication (scalar only)

template <size_t N, typename T>
Vector<N, T> operator *(const Vector<N, T>& vec, const T scalar) {
	return detail::scalarOperator<std::multiplies<T>>(vec, scalar);
}


template <size_t N, typename T>
Vector<N, T>& operator *=(Vector<N, T>& vec, const T scalar) {
	return detail::scalarAssignOperator<std::multiplies<T>>(vec, scalar);
}


// ---- Division (scalar only)

template <size_t N, typename T>
Vector<N, T> operator /(const Vector<N, T>& vec, const T scalar) {
	return detail::scalarOperator<std::divides<T>>(vec, scalar);
}


template <size_t N, typename T>
Vector<N, T>& operator /=(Vector<N, T>& vec, const T scalar) {
	return detail::scalarAssignOperator<std::divides<T>>(vec, scalar);
}



	
} // ns math
} // ns stardazed

#endif
