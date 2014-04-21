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


template <size_t RowCount, size_t ColCount, typename T = float>
struct Matrix {
	static_assert(RowCount > 0 && ColCount > 0, "RowCount > 0 && ColCount > 0");

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
	template <size_t RC = RowCount, size_t CC = ColCount>
	explicit constexpr Matrix(T diag, std::enable_if_t<RC == CC>* = nullptr) {
		T* cell = &data[0];
		const auto stride = ColCount + 1;
		
		for (size_t ix=0; ix < size(); ++ix)
			*(cell++) = (ix % stride == 0) ? diag : 0;
	}

	// default constructor creates 0-initialized matrix
	Matrix() {
		std::fill(dataBegin(), dataEnd(), 0);
	}

	Matrix(std::initializer_list<T> values) {
		const auto maxCells = RowCount * ColCount;
		const auto count = values.size();
		assert(count <= maxCells);

		std::copy(values.begin(), values.end(), data);
		
		if (count < maxCells)
			std::fill_n(dataBegin() + count, maxCells - count, T(0));
	}


	constexpr RowType& operator[](const size_t row) {
		assert(row < RowCount);
		return rows[row];
	}
	
	constexpr const RowType& operator[](const size_t row) const {
		assert(row < RowCount);
		return rows[row];
	}

	// standard begin and end return row type
	constexpr RowType* begin() { return &rows[0]; }
	constexpr RowType* end() { return &rows[0] + RowCount; }
	constexpr const RowType* begin() const { return &rows[0]; }
	constexpr const RowType* end() const { return &rows[0] + RowCount; }

	// dataBegin & dataEnd for component-level access
	constexpr T* dataBegin() { return &data[0]; }
	constexpr T* dataEnd() { return &data[0] + size(); }
	constexpr const T* dataBegin() const { return &data[0]; }
	constexpr const T* dataEnd() const { return &data[0] + size(); }
};


// ---- Type aliases

using Mat2 = Matrix<2, 2>;
using Mat3 = Matrix<3, 3>;
using Mat4 = Matrix<4, 4>;


// ---- Non-member row and column iterators

template <bool UseRow, size_t M, size_t N, typename T>
class MatrixIterator : public std::iterator<std::forward_iterator_tag, T> {
	const size_t stride = UseRow ? 1 : N;
	const size_t skip   = UseRow ? N : 1;
	const Matrix<M, N, T>& matrix;
	size_t offset;
public:
	constexpr MatrixIterator(const Matrix<M, N, T>& matrix, size_t index)
	: matrix{matrix}, offset{ index * skip } {
		assert(offset < M * N + 1);
	}
	
	constexpr T operator *() const { return matrix.data[offset]; }
	constexpr const MatrixIterator<UseRow, M, N, T>& operator ++() { offset += stride; return *this; }
	constexpr MatrixIterator<UseRow, M, N, T> operator ++(int) { auto ret = *this; offset += stride; return ret; }
	
	constexpr bool operator ==(const MatrixIterator<UseRow, M, N, T>& other) { return offset == other.offset; }
	constexpr bool operator !=(const MatrixIterator<UseRow, M, N, T>& other) { return ! this->operator==(other); }
};


template <size_t M, size_t N, typename T>
using MatrixRowIterator = MatrixIterator<true, M, N, T>;

template <size_t M, size_t N, typename T>
using MatrixColumnIterator = MatrixIterator<false, M, N, T>;


template <size_t M, size_t N, typename T>
MatrixRowIterator<M, N, T> rowBegin(const Matrix<M,N,T>& matrix, size_t row) { return { matrix, row }; }

template <size_t M, size_t N, typename T>
MatrixRowIterator<M, N, T> rowEnd(const Matrix<M,N,T>& matrix, size_t row) {
	MatrixRowIterator<M, N, T> it = { matrix, row };
	return std::next(it, N);
}


template <size_t M, size_t N, typename T>
MatrixColumnIterator<M, N, T> colBegin(const Matrix<M,N,T>& matrix, size_t col) { return { matrix, col }; }

template <size_t M, size_t N, typename T>
MatrixColumnIterator<M, N, T> colEnd(const Matrix<M,N,T>& matrix, size_t col) {
	MatrixColumnIterator<M, N, T> it = { matrix, col };
	return std::next(it, M);
}


// ---- Equality test (mostly for tests)

template <size_t M, size_t N, typename T>
constexpr bool operator==(const Matrix<M, N, T>& a, const Matrix<M, N, T>& b) {
	return std::equal(a.dataBegin(), a.dataEnd(), b.dataBegin());
}


template <size_t M, size_t N, typename T>
constexpr bool operator!=(const Matrix<M, N, T>& a, const Matrix<M, N, T>& b) {
	return !(a == b);
}


// ---- Matrix-Matrix Addition

template <size_t M, size_t N, typename T>
Matrix<M, N, T> operator +(const Matrix<M, N, T>& a, const Matrix<M, N, T>& b) {
	auto result = a;
	result += b;
	return result;
}


template <size_t M, size_t N, typename T>
Matrix<M, N, T>& operator +=(Matrix<M, N, T>& a, const Matrix<M, N, T>& b) {
	std::transform(a.dataBegin(), a.dataEnd(), b.dataBegin(), a.dataBegin(), std::plus<T>());
	return a;
}


// ---- Matrix-Matrix Subtraction

template <size_t M, size_t N, typename T>
Matrix<M, N, T> operator -(const Matrix<M, N, T>& a, const Matrix<M, N, T>& b) {
	auto result = a;
	result -= b;
	return result;
}


template <size_t M, size_t N, typename T>
Matrix<M, N, T>& operator -=(Matrix<M, N, T>& a, const Matrix<M, N, T>& b) {
	std::transform(a.dataBegin(), a.dataEnd(), b.dataBegin(), a.dataBegin(), std::minus<T>());
	return a;
}


// ---- Matrix-Scalar Multiplication

template <size_t M, size_t N, typename T, typename S>
std::enable_if_t<std::is_convertible<S, T>::value, Matrix<M, N, T>>
operator *(const Matrix<M, N, T>& mat, const S scalar) {
	auto result = mat;
	result *= scalar;
	return result;
}


template <size_t M, size_t N, typename T, typename S>
Matrix<M, N, T>& operator *=(Matrix<M, N, T>& mat, const S scalar) {
	std::transform(mat.dataBegin(), mat.dataEnd(), mat.dataBegin(), [scalar](T a) {
		return a * scalar;
	});
	return mat;
}


// ---- Matrix-Scalar Division

template <size_t M, size_t N, typename T, typename S>
std::enable_if_t<std::is_convertible<S, T>::value, Matrix<M, N, T>>
operator /(const Matrix<M, N, T>& mat, const S scalar) {
	auto result = mat;
	result /= scalar;
	return result;
}


template <size_t M, size_t N, typename T, typename S>
Matrix<M, N, T>& operator /=(Matrix<M, N, T>& mat, const S scalar) {
	std::transform(mat.dataBegin(), mat.dataEnd(), mat.dataBegin(), [scalar](T a) {
		return a / scalar;
	});
	return mat;
}


// ---- Matrix-Vector Multiplication

template <size_t N, typename T>
Vector<N, T> operator *(const Matrix<N, N, T>& mat, const Vector<N, T>& vec) {
	Vector<N, T> result;

	for (size_t i=0; i<N; ++i) {
		const auto& matRow = mat[i];
		result[i] = std::inner_product(matRow.begin(), matRow.end(), vec.begin(), T{0});
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
		mat[3].x * vec.x + mat[3].y * vec.y + mat[3].z * vec.z + mat[3].w * vec.w
	};
}


template <typename T>
Vector<3, T> constexpr operator *(const Matrix<4, 4, T>& mat, const Vector<3, T> vec) {
	// not a specialization, separate template for m4x4 * v3 expression
	// which is common in 3D contexts; implicitly substitutes T{1} for missing vec.w
	return {
		mat[0].x * vec.x + mat[0].y * vec.y + mat[0].z * vec.z + mat[0].w,
		mat[1].x * vec.x + mat[1].y * vec.y + mat[1].z * vec.z + mat[1].w,
		mat[2].x * vec.x + mat[2].y * vec.y + mat[2].z * vec.z + mat[2].w
	};
}


// ---- Matrix-Matrix multiplication

template <size_t M, size_t N, size_t P, typename T>
Matrix<M, P, T> operator *(const Matrix<M, N, T>& a, const Matrix<N, P, T>& b) {
	Matrix<M, P, T> result;

	for (size_t i=0; i < M; ++i)
		for (size_t j=0; j < P; ++j)
			result[i][j] = std::inner_product(colBegin(a, j), colEnd(a, j), rowBegin(b, i), T{0});
	
	return result;
}


template <typename T>
Matrix<2, 2, T> operator *(const Matrix<2, 2, T>& a, const Matrix<2, 2, T>& b) {
	return {
		a[0][0] * b[0][0] + a[1][0] * b[0][1],
		a[0][1] * b[0][0] + a[1][1] * b[0][1],

		a[0][0] * b[1][0] + a[1][0] * b[1][1],
		a[0][1] * b[1][0] + a[1][1] * b[1][1],
	};
}


template <typename T>
Matrix<3, 3, T> operator *(const Matrix<3, 3, T>& a, const Matrix<3, 3, T>& b) {
	return {
		a[0][0] * b[0][0] + a[1][0] * b[0][1] + a[2][0] * b[0][2],
		a[0][1] * b[0][0] + a[1][1] * b[0][1] + a[2][1] * b[0][2],
		a[0][2] * b[0][0] + a[1][2] * b[0][1] + a[2][2] * b[0][2],
		
		a[0][0] * b[1][0] + a[1][0] * b[1][1] + a[2][0] * b[1][2],
		a[0][1] * b[1][0] + a[1][1] * b[1][1] + a[2][1] * b[1][2],
		a[0][2] * b[1][0] + a[1][2] * b[1][1] + a[2][2] * b[1][2],

		a[0][0] * b[2][0] + a[1][0] * b[2][1] + a[2][0] * b[2][2],
		a[0][1] * b[2][0] + a[1][1] * b[2][1] + a[2][1] * b[2][2],
		a[0][2] * b[2][0] + a[1][2] * b[2][1] + a[2][2] * b[2][2],
	};
}


template <typename T>
Matrix<4, 4, T> operator *(const Matrix<4, 4, T>& a, const Matrix<4, 4, T>& b) {
	return {
		a[0][0] * b[0][0] + a[1][0] * b[0][1] + a[2][0] * b[0][2] + a[3][0] * b[0][3],
		a[0][1] * b[0][0] + a[1][1] * b[0][1] + a[2][1] * b[0][2] + a[3][1] * b[0][3],
		a[0][2] * b[0][0] + a[1][2] * b[0][1] + a[2][2] * b[0][2] + a[3][2] * b[0][3],
		a[0][3] * b[0][0] + a[1][3] * b[0][1] + a[2][3] * b[0][2] + a[3][3] * b[0][3],

		a[0][0] * b[1][0] + a[1][0] * b[1][1] + a[2][0] * b[1][2] + a[3][0] * b[1][3],
		a[0][1] * b[1][0] + a[1][1] * b[1][1] + a[2][1] * b[1][2] + a[3][1] * b[1][3],
		a[0][2] * b[1][0] + a[1][2] * b[1][1] + a[2][2] * b[1][2] + a[3][2] * b[1][3],
		a[0][3] * b[1][0] + a[1][3] * b[1][1] + a[2][3] * b[1][2] + a[3][3] * b[1][3],

		a[0][0] * b[2][0] + a[1][0] * b[2][1] + a[2][0] * b[2][2] + a[3][0] * b[2][3],
		a[0][1] * b[2][0] + a[1][1] * b[2][1] + a[2][1] * b[2][2] + a[3][1] * b[2][3],
		a[0][2] * b[2][0] + a[1][2] * b[2][1] + a[2][2] * b[2][2] + a[3][2] * b[2][3],
		a[0][3] * b[2][0] + a[1][3] * b[2][1] + a[2][3] * b[2][2] + a[3][3] * b[2][3],

		a[0][0] * b[3][0] + a[1][0] * b[3][1] + a[2][0] * b[3][2] + a[3][0] * b[3][3],
		a[0][1] * b[3][0] + a[1][1] * b[3][1] + a[2][1] * b[3][2] + a[3][1] * b[3][3],
		a[0][2] * b[3][0] + a[1][2] * b[3][1] + a[2][2] * b[3][2] + a[3][2] * b[3][3],
		a[0][3] * b[3][0] + a[1][3] * b[3][1] + a[2][3] * b[3][2] + a[3][3] * b[3][3]
	};
}


// ---- Transpose

template <size_t M, size_t N, typename T>
Matrix<N, M, T> transpose(const Matrix<M, N, T>& mat) {
	Matrix<N, M, T> result;

	for (size_t ix=0; ix < N; ++ix) {
		std::copy(colBegin(mat, ix), colEnd(mat, ix), result[ix].begin());
	}

	return result;
}


template <typename T>
constexpr Matrix<2, 2, T> transpose(const Matrix<2, 2, T>& mat) {
	return {
		mat[0][0], mat[1][0],
		mat[0][1], mat[1][1]
	};
}


template <typename T>
constexpr Matrix<3, 3, T> transpose(const Matrix<3, 3, T>& mat) {
	return {
		mat[0][0], mat[1][0], mat[2][0],
		mat[0][1], mat[1][1], mat[2][1],
		mat[0][2], mat[1][2], mat[2][2],
	};
}


template <typename T>
constexpr Matrix<4, 4, T> transpose(const Matrix<4, 4, T>& mat) {
	return {
		mat[0][0], mat[1][0], mat[2][0], mat[3][0],
		mat[0][1], mat[1][1], mat[2][1], mat[3][1],
		mat[0][2], mat[1][2], mat[2][2], mat[3][2],
		mat[0][3], mat[1][3], mat[2][3], mat[3][3]
	};
}


// ---- Determinant

// generic determinant not implemented, only specified for 2x2, 3x3 and 4x4
template <size_t N, typename T>
Matrix<N, N, T> determinant(const Matrix<N, N, T>& mat);


template <typename T>
constexpr T determinant(const Matrix<2, 2, T>& mat) {
	return mat[0][0] * mat[1][1] - mat[1][0] * mat[0][1];
}


template <typename T>
constexpr T determinant(const Matrix<3, 3, T>& mat) {
	return mat[0][0] * mat[1][1] * mat[2][2] + // aei
	       mat[0][1] * mat[1][2] * mat[2][0] + // bfg
		   mat[0][2] * mat[1][0] * mat[2][1] - // cdh
		   mat[0][2] * mat[1][1] * mat[2][0] - // ceg
		   mat[0][1] * mat[1][0] * mat[2][2] - // bdi
		   mat[0][0] * mat[1][2] * mat[2][1];  // afh
}


template <typename T>
constexpr T determinant(const Matrix<4, 4, T>& mat) {
	return mat[0][0] * mat[1][1] * mat[2][2] * mat[3][3] - mat[0][0] * mat[1][1] * mat[2][3] * mat[3][2]
		 + mat[0][0] * mat[1][2] * mat[2][3] * mat[3][1] - mat[0][0] * mat[1][2] * mat[2][1] * mat[3][3]
		 + mat[0][0] * mat[1][3] * mat[2][1] * mat[3][2] - mat[0][0] * mat[1][3] * mat[2][2] * mat[3][1]
		 - mat[0][1] * mat[1][2] * mat[2][3] * mat[3][0] + mat[0][1] * mat[1][2] * mat[2][0] * mat[3][3]
		 - mat[0][1] * mat[1][3] * mat[2][0] * mat[3][2] + mat[0][1] * mat[1][3] * mat[2][2] * mat[3][0]
		 - mat[0][1] * mat[1][0] * mat[2][2] * mat[3][3] + mat[0][1] * mat[1][0] * mat[2][3] * mat[3][2]
		 + mat[0][2] * mat[1][3] * mat[2][0] * mat[3][1] - mat[0][2] * mat[1][3] * mat[2][1] * mat[3][0]
		 + mat[0][2] * mat[1][0] * mat[2][1] * mat[3][3] - mat[0][2] * mat[1][0] * mat[2][3] * mat[3][1]
		 + mat[0][2] * mat[1][1] * mat[2][3] * mat[3][0] - mat[0][2] * mat[1][1] * mat[2][0] * mat[3][3]
		 - mat[0][3] * mat[1][0] * mat[2][1] * mat[3][2] + mat[0][3] * mat[1][0] * mat[2][2] * mat[3][1]
		 - mat[0][3] * mat[1][1] * mat[2][2] * mat[3][0] + mat[0][3] * mat[1][1] * mat[2][0] * mat[3][2]
		 - mat[0][3] * mat[1][2] * mat[2][0] * mat[3][1] + mat[0][3] * mat[1][2] * mat[2][1] * mat[3][0];
}


// ---- Inverse, 2x2, 3x3 and 4x4 only
// The following 3 implementations were copied over with minor modifications
// from GLM library 0.9.5 by Christophe Riccio
// Copyright (c) 2005 - 2014 G-Truc Creation (www.g-truc.net)

template <size_t N, typename T>
Matrix<N, N, T> inverse(const Matrix<N, N, T>& mat);


template <typename T>
constexpr Matrix<2, 2, T> inverse(const Matrix<2, 2, T>& m) {
	T oneOverDeterminant = static_cast<T>(1) / determinant(m);

	return {
		+ m[1][1] * oneOverDeterminant,
		- m[0][1] * oneOverDeterminant,
		- m[1][0] * oneOverDeterminant,
		+ m[0][0] * oneOverDeterminant
	};
}


template <typename T>
constexpr Matrix<3, 3, T> inverse(const Matrix<3, 3, T>& m) {
	T oneOverDeterminant = static_cast<T>(1) / determinant(m);

	return {
		+ (m[1][1] * m[2][2] - m[2][1] * m[1][2]) * oneOverDeterminant,
		- (m[0][1] * m[2][2] - m[2][1] * m[0][2]) * oneOverDeterminant,
		+ (m[0][1] * m[1][2] - m[1][1] * m[0][2]) * oneOverDeterminant,
		- (m[1][0] * m[2][2] - m[2][0] * m[1][2]) * oneOverDeterminant,
		+ (m[0][0] * m[2][2] - m[2][0] * m[0][2]) * oneOverDeterminant,
		- (m[0][0] * m[1][2] - m[1][0] * m[0][2]) * oneOverDeterminant,
		+ (m[1][0] * m[2][1] - m[2][0] * m[1][1]) * oneOverDeterminant,
		- (m[0][0] * m[2][1] - m[2][0] * m[0][1]) * oneOverDeterminant,
		+ (m[0][0] * m[1][1] - m[1][0] * m[0][1]) * oneOverDeterminant
	};
}


template <typename T>
Matrix<4, 4, T> inverse(Matrix<4, 4, T> mat) {
	T coef00 = mat[2][2] * mat[3][3] - mat[3][2] * mat[2][3];
	T coef02 = mat[1][2] * mat[3][3] - mat[3][2] * mat[1][3];
	T coef03 = mat[1][2] * mat[2][3] - mat[2][2] * mat[1][3];
	
	T coef04 = mat[2][1] * mat[3][3] - mat[3][1] * mat[2][3];
	T coef06 = mat[1][1] * mat[3][3] - mat[3][1] * mat[1][3];
	T coef07 = mat[1][1] * mat[2][3] - mat[2][1] * mat[1][3];
	
	T coef08 = mat[2][1] * mat[3][2] - mat[3][1] * mat[2][2];
	T coef10 = mat[1][1] * mat[3][2] - mat[3][1] * mat[1][2];
	T coef11 = mat[1][1] * mat[2][2] - mat[2][1] * mat[1][2];
	
	T coef12 = mat[2][0] * mat[3][3] - mat[3][0] * mat[2][3];
	T coef14 = mat[1][0] * mat[3][3] - mat[3][0] * mat[1][3];
	T coef15 = mat[1][0] * mat[2][3] - mat[2][0] * mat[1][3];
	
	T coef16 = mat[2][0] * mat[3][2] - mat[3][0] * mat[2][2];
	T coef18 = mat[1][0] * mat[3][2] - mat[3][0] * mat[1][2];
	T coef19 = mat[1][0] * mat[2][2] - mat[2][0] * mat[1][2];
	
	T coef20 = mat[2][0] * mat[3][1] - mat[3][0] * mat[2][1];
	T coef22 = mat[1][0] * mat[3][1] - mat[3][0] * mat[1][1];
	T coef23 = mat[1][0] * mat[2][1] - mat[2][0] * mat[1][1];
	
	Vector<4, T> fac0(coef00, coef00, coef02, coef03);
	Vector<4, T> fac1(coef04, coef04, coef06, coef07);
	Vector<4, T> fac2(coef08, coef08, coef10, coef11);
	Vector<4, T> fac3(coef12, coef12, coef14, coef15);
	Vector<4, T> fac4(coef16, coef16, coef18, coef19);
	Vector<4, T> fac5(coef20, coef20, coef22, coef23);
	
	Vector<4, T> vec0(mat[1][0], mat[0][0], mat[0][0], mat[0][0]);
	Vector<4, T> vec1(mat[1][1], mat[0][1], mat[0][1], mat[0][1]);
	Vector<4, T> vec2(mat[1][2], mat[0][2], mat[0][2], mat[0][2]);
	Vector<4, T> vec3(mat[1][3], mat[0][3], mat[0][3], mat[0][3]);
	
	Vector<4, T> inv0(vec1 * fac0 - vec2 * fac1 + vec3 * fac2);
	Vector<4, T> inv1(vec0 * fac0 - vec2 * fac3 + vec3 * fac4);
	Vector<4, T> inv2(vec0 * fac1 - vec1 * fac3 + vec3 * fac5);
	Vector<4, T> inv3(vec0 * fac2 - vec1 * fac4 + vec2 * fac5);
	
	Vector<4, T> signA(+1, -1, +1, -1);
	Vector<4, T> signB(-1, +1, -1, +1);
	Matrix<4, 4, T> result = make4x4MatrixFromColumns(inv0 * signA, inv1 * signB, inv2 * signA, inv3 * signB);
	
	Vector<4, T> row0(result[0][0], result[1][0], result[2][0], result[3][0]);
	
	Vector<4, T> dot0(mat[0] * row0);
	T dot1 = dot0.x + dot0.y + dot0.z + dot0.w;
	T oneOverDeterminant = static_cast<T>(1) / dot1;
	
	return result * oneOverDeterminant;
}



// ---- Matrix construction from rows or columns (explicit)

template <size_t N, typename T = float>
Matrix<N, N, T> makeSquareMatrixFromColumns(std::initializer_list<Vector<N, T>> cols) {
	assert(cols.size() == N);
	Matrix<N, N, T> result;

	std::copy(cols.begin(), cols.end(), result.begin());
	return result;
}


template <size_t N, typename T = float>
Matrix<N, N, T> makeSquareMatrixFromRows(std::initializer_list<Vector<N, T>> rows) {
	return transpose(makeSquareMatrixFromColumns(rows));
}


template <typename T>
Matrix<2, 2, T> make2x2MatrixFromColumns(const Vector<2, T>& c0, const Vector<2, T>& c1) {
	Matrix<2, 2, T> result;
	result[0] = c0;
	result[1] = c1;
	return result;
}


template <typename T>
Matrix<2, 2, T> make2x2MatrixFromRows(const Vector<2, T>& r0, const Vector<2, T>& r1) {
	return transpose(make2x2MatrixFromColumns(r0, r1));
}


template <typename T>
Matrix<3, 3, T> make3x3MatrixFromColumns(const Vector<3, T>& c0, const Vector<3, T>& c1, const Vector<3, T>& c2) {
	Matrix<3, 3, T> result;
	result[0] = c0;
	result[1] = c1;
	result[2] = c2;
	return result;
}


template <typename T>
Matrix<3, 3, T> make3x3MatrixFromRows(const Vector<3, T>& r0, const Vector<3, T>& r1, const Vector<3, T>& r2) {
	return transpose(make3x3MatrixFromColumns(r0, r1, r2));
}


template <typename T>
Matrix<4, 4, T> make4x4MatrixFromColumns(const Vector<4, T>& c0, const Vector<4, T>& c1, const Vector<4, T>& c2, const Vector<4, T>& c3) {
	Matrix<4, 4, T> result;
	result[0] = c0;
	result[1] = c1;
	result[2] = c2;
	result[3] = c3;
	return result;
}


template <typename T>
Matrix<4, 4, T> make4x4MatrixFromRows(const Vector<3, T>& r0, const Vector<3, T>& r1, const Vector<3, T>& r2, const Vector<4, T>& r3) {
	return transpose(make4x4MatrixFromColumns(r0, r1, r2, r3));
}


// ---- Extract sub-matrix (always from top-left corner)

template <size_t P, size_t Q, size_t M, size_t N, typename T>
Matrix<P, Q, T> extractSubMatrix(const Matrix<M, N, T>& mat) {
	static_assert(P <= M, "P <= M");
	static_assert(Q <= N, "Q <= N");
	
	Matrix<P, Q, T> result;
	auto resIt = result.begin();
	for (auto from = mat.begin(), to = from + P; from != to; ++from, ++resIt) {
		std::copy(from->begin(), from->begin() + Q, resIt->begin());
	}
	return result;
}


// fully specialized float and double 3x3 from 4x4 implementation (partial spec of temp fn not allowed)
template <>
Matrix<3, 3, float> extractSubMatrix<3,3,4,4,float>(const Matrix<4, 4, float>& mat) {
	return {
		mat[0][0],mat[0][1],mat[0][2],
		mat[1][0],mat[1][1],mat[1][2],
		mat[2][0],mat[2][1],mat[2][2]
	};
}


template <>
Matrix<3, 3, double> extractSubMatrix<3,3,4,4,double>(const Matrix<4, 4, double>& mat) {
	return {
		mat[0][0],mat[0][1],mat[0][2],
		mat[1][0],mat[1][1],mat[1][2],
		mat[2][0],mat[2][1],mat[2][2]
	};
}


} // ns math
} // ns stardazed

#endif
