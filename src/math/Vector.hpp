// ------------------------------------------------------------------
// math::Vector, revision the 3rd - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_MATH_VECTOR_H
#define SD_MATH_VECTOR_H

#include "system/Config.hpp"

#include <initializer_list>
#include <algorithm>
#include <functional>
#include <numeric>
#include <cmath>

namespace stardazed {
namespace math {


namespace detail {
	// ---- Vector shared data access
	
	template <typename VecImp, size_t N, typename T>
	struct VectorBase {
		using ValueType = T;

		constexpr T& operator[](const size_t index) {
			assert(index < N);
			return static_cast<VecImp*>(this)->data[index];
		}
		
		constexpr T operator[](const size_t index) const {
			assert(index < N);
			return static_cast<const VecImp*>(this)->data[index];
		}
		
		constexpr size_t size() const { return N; }
		constexpr T* begin() { return &static_cast<VecImp*>(this)->data[0]; }
		constexpr T* end() { return &static_cast<VecImp*>(this)->data[0] + N; }
		constexpr const T* begin() const { return &static_cast<const VecImp*>(this)->data[0]; }
		constexpr const T* end() const { return &static_cast<const VecImp*>(this)->data[0] + N; }
	};
}


template <size_t N, typename T = float>
struct Vector : public detail::VectorBase<Vector<N, T>, N, T> {
	T data[N];

	explicit Vector(const T fill) {
		std::fill_n(data, N, fill);
	}
	Vector() : Vector(T(0)) {}

	Vector(std::initializer_list<T> values) {
		assert(values.size() <= N);

		auto from = values.begin();
		auto count = std::min(N, values.size());
		auto to = from + count;
		std::copy(from, to, data);
		
		if (count < N)
			std::fill_n(&data[0] + count, N - count, T(0));
	}
};


// type aliases
using Vec2 = Vector<2>;
using Vec3 = Vector<3>;
using Vec4 = Vector<4>;


// Vector class specializations (N in [2,3,4])
template <typename T>
struct Vector<2, T> : public detail::VectorBase<Vector<2, T>, 2, T> {
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
struct Vector<3, T> : public detail::VectorBase<Vector<3, T>, 3, T> {
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
struct Vector<4, T> : public detail::VectorBase<Vector<4, T>, 4, T> {
	union {
		T data[4];
		struct { T x, y, z, w; };
		struct { T r, g, b, a; };
		Vector<2, T> xy;
		Vector<2, T> uv;
		Vector<3, T> xyz;
		Vector<3, T> rgb;
	};
	
	explicit constexpr Vector(const T fill) : x(fill), y(fill), z(fill), w(fill) {}
	constexpr Vector(const T x, const T y, const T z, const T w) : x(x), y(y), z(z), w(w) {}
	explicit constexpr Vector(const Vector<3, T>& xyz, const T w) : x(xyz.x), y(xyz.y), z(xyz.z), w(w) {}
	constexpr Vector() : Vector(T(0)) {}
};



// implementation of generic and specialized component-wise operators for Vectors
namespace detail {
	template <typename Op, size_t N, typename T>
	Vector<N, T>& componentWiseAssignOperator(Vector<N, T>& a, const Vector<N, T>& b) {
		std::transform(a.begin(), a.end(), b.begin(), a.begin(), [op = Op()](auto va, auto vb) {
			return op(va, vb);
		});
		return a;
	}
	
	template <typename Op, typename T>
	Vector<2, T>& componentWiseAssignOperator(Vector<2, T>& a, const Vector<2, T>& b) {
		auto op = Op();
		a[0] = op(a[0], b[0]);
		a[1] = op(a[1], b[1]);
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
	
	template <typename Op, typename T>
	Vector<4, T>& componentWiseAssignOperator(Vector<4, T>& a, const Vector<4, T>& b) {
		auto op = Op();
		a[0] = op(a[0], b[0]);
		a[1] = op(a[1], b[1]);
		a[2] = op(a[2], b[2]);
		a[3] = op(a[3], b[3]);
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


template <size_t N, typename T, typename S>
std::enable_if_t<std::is_convertible<S, T>::value, Vector<N, T>>
operator +(const Vector<N, T>& vec, const S scalar) {
	return detail::scalarOperator<std::plus<T>>(vec, static_cast<T>(scalar));
}


template <size_t N, typename T, typename S>
std::enable_if_t<std::is_convertible<S, T>::value, Vector<N, T>&>
operator +=(Vector<N, T>& vec, const S scalar) {
	return detail::scalarAssignOperator<std::plus<T>>(vec, static_cast<T>(scalar));
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


template <size_t N, typename T, typename S>
std::enable_if_t<std::is_convertible<S, T>::value, Vector<N, T>>
operator -(const Vector<N, T>& vec, const S scalar) {
	return detail::scalarOperator<std::minus<T>>(vec, static_cast<T>(scalar));
}


template <size_t N, typename T, typename S>
std::enable_if_t<std::is_convertible<S, T>::value, Vector<N, T>&>
operator -=(Vector<N, T>& vec, const S scalar) {
	return detail::scalarAssignOperator<std::minus<T>>(vec, static_cast<T>(scalar));
}

	
// ---- Multiplication

template <size_t N, typename T>
Vector<N, T> operator *(const Vector<N, T>& a, const Vector<N, T>& b) {
	return detail::componentWiseOperator<std::multiplies<T>>(a, b);
}


template <size_t N, typename T>
Vector<N, T>& operator *=(Vector<N, T>& a, const Vector<N, T>& b) {
	return detail::componentWiseAssignOperator<std::multiplies<T>>(a, b);
}


template <size_t N, typename T, typename S>
std::enable_if_t<std::is_convertible<S, T>::value, Vector<N, T>>
operator *(const Vector<N, T>& vec, const S scalar) {
	return detail::scalarOperator<std::multiplies<T>>(vec, static_cast<T>(scalar));
}


template <size_t N, typename T, typename S>
std::enable_if_t<std::is_convertible<S, T>::value, Vector<N, T>&>
operator *=(Vector<N, T>& vec, const S scalar) {
	return detail::scalarAssignOperator<std::multiplies<T>>(vec, static_cast<T>(scalar));
}


// ---- Division

template <size_t N, typename T>
Vector<N, T> operator /(const Vector<N, T>& a, const Vector<N, T>& b) {
	return detail::componentWiseOperator<std::divides<T>>(a, b);
}


template <size_t N, typename T>
Vector<N, T>& operator /=(Vector<N, T>& a, const Vector<N, T>& b) {
	return detail::componentWiseAssignOperator<std::divides<T>>(a, b);
}


template <size_t N, typename T, typename S>
std::enable_if_t<std::is_convertible<S, T>::value, Vector<N, T>>
operator /(const Vector<N, T>& vec, const S scalar) {
	return detail::scalarOperator<std::divides<T>>(vec, static_cast<T>(scalar));
}


template <size_t N, typename T, typename S>
std::enable_if_t<std::is_convertible<S, T>::value, Vector<N, T>&>
operator /=(Vector<N, T>& vec, const S scalar) {
	return detail::scalarAssignOperator<std::divides<T>>(vec, static_cast<T>(scalar));
}

	
// ---- Unary minus

template <size_t N, typename T>
Vector<N, T> operator -(const Vector<N, T>& vec) {
	auto result = vec;
	std::transform(result.begin(), result.end(), result.begin(), std::negate<T>());
	return result;
}


template <typename T>
constexpr Vector<2, T> operator -(const Vector<2, T>& vec) {
	return { -vec.x, -vec.y };
}


template <typename T>
constexpr Vector<3, T> operator -(const Vector<3, T>& vec) {
	return { -vec.x, -vec.y, -vec.z };
}


template <typename T>
constexpr Vector<4, T> operator -(const Vector<4, T>& vec) {
	return { -vec.x, -vec.y, -vec.z, -vec.w };
}


// ---- Exact component-wise comparison

template <size_t N, typename T>
bool operator ==(const Vector<N, T>& a, const Vector<N, T>& b) {
	return std::equal(a.begin(), a.end(), b.begin());
}


template <size_t N, typename T>
constexpr bool operator !=(const Vector<N, T>& a, const Vector<N, T>& b) {
	return ! (a == b);
}


template <typename T>
constexpr bool operator ==(const Vector<2, T>& a, const Vector<2, T>& b) {
	return (a.x == b.x) && (a.y == b.y);
}


template <typename T>
constexpr bool operator ==(const Vector<3, T>& a, const Vector<3, T>& b) {
	return (a.x == b.x) && (a.y == b.y) && (a.z == b.z);
}


template <typename T>
constexpr bool operator ==(const Vector<4, T>& a, const Vector<4, T>& b) {
	return (a.x == b.x) && (a.y == b.y) && (a.z == b.z) && (a.w == b.w);
}

	
// ---- Dot product

template <size_t N, typename T>
T dot(const Vector<N, T>& a, const Vector<N, T>& b) {
	return std::inner_product(a.begin(), a.end(), b.begin(), T{0});
}


template <typename T>
constexpr T dot(const Vector<2, T>& a, const Vector<2, T>& b) {
	return (a.x * b.x) + (a.y * b.y);
}


template <typename T>
constexpr T dot(const Vector<3, T>& a, const Vector<3, T>& b) {
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}


template <typename T>
constexpr T dot(const Vector<4, T>& a, const Vector<4, T>& b) {
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w);
}


// ---- Cross product

template <typename T>
constexpr Vector<3, T> cross(const Vector<3, T>& a, const Vector<3, T>& b) {
	return {
		(a.y * b.z) - (a.z * b.y),
		(a.z * b.x) - (a.x * b.z),
		(a.x * b.y) - (a.y * b.x)
	};
}


// ---- OrthoNormalize

template <typename T>
void orthoNormalize(Vector<3, T>& normal, Vector<3, T>& tangent) {
	normalizeInPlace(normal);
	auto proj = normal * dot(tangent, normal);
	tangent -= proj;
	normalizeInPlace(tangent);
}



// ---- Length

template <size_t N, typename T>
T lengthSquared(const Vector<N, T>& vec) {
	return std::inner_product(vec.begin(), vec.end(), vec.begin(), T(0));
}


template <typename T>
constexpr T lengthSquared(const Vector<2, T>& vec) {
	return vec.x * vec.x + vec.y * vec.y;
}


template <typename T>
constexpr T lengthSquared(const Vector<3, T>& vec) {
	return vec.x * vec.x + vec.y * vec.y + vec.z * vec.z;
}


template <typename T>
constexpr T lengthSquared(const Vector<4, T>& vec) {
	return vec.x * vec.x + vec.y * vec.y + vec.z * vec.z + vec.w * vec.w;
}


template <size_t N, typename T>
T length(const Vector<N, T>& vec) {
	return std::sqrt(lengthSquared(vec));
}


template <typename T>
constexpr T length(const Vector<2, T>& vec) {
	return std::sqrt(lengthSquared(vec));
}


template <typename T>
constexpr T length(const Vector<3, T>& vec) {
	return std::sqrt(lengthSquared(vec));
}


template <typename T>
constexpr T length(const Vector<4, T>& vec) {
	return std::sqrt(lengthSquared(vec));
}


// ---- Normalize (in-place and pure)

template <size_t N, typename T>
Vector<N, T>& normalizeInPlace(Vector<N, T>& vec) {
	vec /= length(vec);
	return vec;
}


template <size_t N, typename T>
Vector<N, T> normalize(const Vector<N, T>& vec) {
	auto normal = vec;
	return normalizeInPlace(normal);
}


// ---- Swappable

template <size_t N, typename T>
void swap(Vector<N, T>& a, Vector<N, T>& b) {
	std::swap<T, N>(a.data, b.data);
}


	
} // ns math
} // ns stardazed

#endif
