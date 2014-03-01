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
	// ---- Matrix shared data access
	
	template <typename MatImp, size_t Rows, size_t Cols, typename T>
	struct MatrixBase {
		using ValueType = Vector<Cols, T>;
		
		MatrixBase() = default;
		MatrixBase(std::initializer_list<T> values) {
			T* data = &static_cast<MatImp*>(this)->data[0][0];
			auto maxCells = Rows * Cols;

			auto from = values.begin();
			auto count = std::min(maxCells, values.size());
			auto to = from + count;
			std::copy(from, to, data);
			
			if (count < maxCells)
				std::fill_n(data + count, maxCells - count, T(0));
		}

		constexpr ValueType& operator[](const size_t row) {
			assert(row < Rows);
			return static_cast<MatImp*>(this)->data[row];
		}
		
		constexpr const ValueType& operator[](const size_t row) const {
			assert(row < Rows);
			return static_cast<const MatImp*>(this)->data[row];
		}
		
		constexpr size_t size() const { return Rows; }
		constexpr size_t rows() const { return Rows; }
		constexpr size_t cols() const { return Cols; }

		constexpr ValueType* begin() { return &static_cast<MatImp*>(this)->data[0]; }
		constexpr ValueType* end() { return &static_cast<MatImp*>(this)->data[0] + Rows; }
		constexpr const ValueType* begin() const { return &static_cast<const MatImp*>(this)->data[0]; }
		constexpr const ValueType* end() const { return &static_cast<const MatImp*>(this)->data[0] + Rows; }
	};
}


template <size_t Rows, size_t Cols, typename T = float>
struct Matrix : public detail::MatrixBase<Matrix<Rows, Cols, T>, Rows, Cols, T> {
	using ValueType = Vector<Cols, T>;

	ValueType data[Rows];
	
	using detail::MatrixBase<Matrix<Rows, Cols, T>, Rows, Cols, T>::MatrixBase;
};


} // ns math
} // ns stardazed

#endif
