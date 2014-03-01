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
	// ---- Matrix shared data access and constructors
	
	template <typename MatImp, size_t Rows, size_t Cols, typename T>
	struct MatrixBase {
		using RowType = Vector<Cols, T>;

		MatrixBase() = default;
		MatrixBase(std::initializer_list<T> values) {
			assert(values.size() <= Rows * Cols);

			T* data = &static_cast<MatImp*>(this)->data[0];
			auto maxCells = Rows * Cols;

			auto from = values.begin();
			auto count = std::min(maxCells, values.size());
			auto to = from + count;
			std::copy(from, to, data);
			
			if (count < maxCells)
				std::fill_n(data + count, maxCells - count, T(0));
		}

		// enable diagonal constructor only for square matrices
		template <typename = std::enable_if_t<Rows == Cols>>
		explicit constexpr MatrixBase(T diag) {
			T* data = &static_cast<MatImp*>(this)->data[0];
			for (size_t ix=0; ix < size(); ++ix)
				*(data++) = (ix % Cols == 0) ? diag : T(0);
		}

		constexpr RowType& operator[](const size_t row) {
			assert(row < Rows);
			return static_cast<MatImp*>(this)->rows[row];
		}
		
		constexpr const RowType& operator[](const size_t row) const {
			assert(row < Rows);
			return static_cast<const MatImp*>(this)->rows[row];
		}
		
		constexpr size_t size() const { return Rows * Cols; }
		constexpr size_t rows() const { return Rows; }
		constexpr size_t cols() const { return Cols; }

		constexpr RowType* begin() { return &static_cast<MatImp*>(this)->rows[0]; }
		constexpr RowType* end() { return &static_cast<MatImp*>(this)->rows[0] + Rows; }
		constexpr const RowType* begin() const { return &static_cast<const MatImp*>(this)->rows[0]; }
		constexpr const RowType* end() const { return &static_cast<const MatImp*>(this)->rows[0] + Rows; }
	};
}


// ---- Generic matrix, can be any shape

template <size_t Rows, size_t Cols, typename T = float>
struct Matrix : public detail::MatrixBase<Matrix<Rows, Cols, T>, Rows, Cols, T> {
	using RowType = Vector<Cols, T>;

	union {
		T data[Rows * Cols];
		RowType rows[Rows];
	};
	
	using detail::MatrixBase<Matrix<Rows, Cols, T>, Rows, Cols, T>::MatrixBase;
};



} // ns math
} // ns stardazed

#endif
