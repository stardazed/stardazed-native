// ------------------------------------------------------------------
// math::Matrix - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "math/Matrix.hpp"

namespace stardazed {
namespace math {


// fully specialized float and double 3x3 from 4x4 implementation (partial spec of temp fn not allowed)
template <>
Matrix<3, 3, float> extractSubMatrix<3,3,4,4,float>(const Matrix<4, 4, float>& mat) {
	return {
		mat[0][0], mat[0][1], mat[0][2],
		mat[1][0], mat[1][1], mat[1][2],
		mat[2][0], mat[2][1], mat[2][2]
	};
}


template <>
Matrix<3, 3, double> extractSubMatrix<3,3,4,4,double>(const Matrix<4, 4, double>& mat) {
	return {
		mat[0][0], mat[0][1], mat[0][2],
		mat[1][0], mat[1][1], mat[1][2],
		mat[2][0], mat[2][1], mat[2][2]
	};
}
	

} // ns math
} // ns stardazed
