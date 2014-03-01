// ------------------------------------------------------------------
// math::Matrix - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_MATH_MATRIX_H
#define SD_MATH_MATRIX_H

#include "Vector.hpp"

#include <initializer_list>
#include <algorithm>
#include <functional>
#include <numeric>
#include <cassert>
#include <cmath>


namespace stardazed {
namespace math {

namespace detail {
	// ---- utility typename to enable methods only for square matrices
	template <size_t RowCount, size_t ColCount>
	using isSquare_t = std::enable_if_t<RowCount == ColCount>;
}


template <size_t RowCount, size_t ColCount, typename T = float>
struct Matrix {
	using RowType = Vector<ColCount, T>;

	union {
		T data[RowCount * ColCount];
		RowType rows[RowCount];
	};

	// pass-through of template vars
	constexpr size_t size() const { return RowCount * ColCount; }
	constexpr size_t rowCount() const { return RowCount; }
	constexpr size_t colCount() const { return ColCount; }
	

	// enable diagonal constructor only for square matrices
	template <typename = detail::isSquare_t<RowCount, ColCount>>
	explicit constexpr Matrix(T diag) {
		T* cell = &data[0];
		const auto stride = ColCount + 1;
		
		for (size_t ix=0, end = size(); ix < end; ++ix)
			*(cell++) = (ix % stride == 0) ? diag : 0;
	}

	// default constructor creates 0-initialized matrix
	Matrix() : Matrix(0) {}

	Matrix(std::initializer_list<T> values) {
		const auto maxCells = RowCount * ColCount;
		const auto count = values.size();
		assert(count <= maxCells);

		std::copy(values.begin(), values.end(), data);
		
		if (count < maxCells)
			std::fill_n(data + count, maxCells - count, T(0));
	}


	constexpr RowType& operator[](const size_t row) {
		assert(row < RowCount);
		return rows[row];
	}
	
	constexpr const RowType& operator[](const size_t row) const {
		assert(row < RowCount);
		return rows[row];
	}
	
	constexpr RowType* begin() { return &rows[0]; }
	constexpr RowType* end() { return &rows[0] + RowCount; }
	constexpr const RowType* begin() const { return &rows[0]; }
	constexpr const RowType* end() const { return &rows[0] + RowCount; }
};


// ---- Type aliases

using Mat2 = Matrix<2, 2>;
using Mat3 = Matrix<3, 3>;
using Mat4 = Matrix<4, 4>;


// ---- Matrix-Vector multiplication

template <size_t N, typename T>
Vector<N, T> operator *(const Matrix<N, N, T>& mat, const Vector<N, T>& vec) {
	Vector<N, T> result;

	for (size_t i=0; i<N; ++i) {
		const auto& matRow = mat[i];
		result[i] = std::inner_product(matRow.begin(), matRow.end(), vec.begin(), 0);
	}
	
	return result;
}


template <typename T>
Vector<2, T> constexpr operator *(const Matrix<2, 2, T>& mat, const Vector<2, T> vec) {
	return {
		mat[0].x * vec.x + mat[0].y * vec.y,
		mat[1].x * vec.x + mat[1].y * vec.y
	};
}


template <typename T>
Vector<3, T> constexpr operator *(const Matrix<3, 3, T>& mat, const Vector<3, T> vec) {
	return {
		mat[0].x * vec.x + mat[0].y * vec.y + mat[0].z * vec.z,
		mat[1].x * vec.x + mat[1].y * vec.y + mat[1].z * vec.z,
		mat[2].x * vec.x + mat[2].y * vec.y + mat[2].z * vec.z
	};
}


template <typename T>
Vector<4, T> constexpr operator *(const Matrix<4, 4, T>& mat, const Vector<4, T> vec) {
	return {
		mat[0].x * vec.x + mat[0].y * vec.y + mat[0].z * vec.z + mat[0].w * vec.w,
		mat[1].x * vec.x + mat[1].y * vec.y + mat[1].z * vec.z + mat[1].w * vec.w,
		mat[2].x * vec.x + mat[2].y * vec.y + mat[2].z * vec.z + mat[2].w * vec.w,
		mat[3].x * vec.x + mat[3].y * vec.y + mat[3].z * vec.z + mat[3].w * vec.w,
	};
}


template <typename T>
Vector<3, T> constexpr operator *(const Matrix<4, 4, T>& mat, const Vector<3, T> vec) {
	// not a specialization, separate template for m4x4 * v3 expression
	// which is common in 3D contexts; implicitly substitutes T{1} for missing vec.w
	return {
		mat[0].x * vec.x + mat[0].y * vec.y + mat[0].z * vec.z + mat[0].w,
		mat[1].x * vec.x + mat[1].y * vec.y + mat[1].z * vec.z + mat[1].w,
		mat[2].x * vec.x + mat[2].y * vec.y + mat[2].z * vec.z + mat[2].w,
	};
}



} // ns math
} // ns stardazed

#endif
