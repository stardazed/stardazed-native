// ------------------------------------------------------------------
// math::Constants.cpp - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "math/Constants.hpp"

namespace stardazed {
namespace math {
		

// explicitly instantiate common Pi and Tau constant templates
namespace {
	__attribute__((unused)) const auto pi_f = Pi<float>;
	__attribute__((unused)) const auto pi_d = Pi<double>;

	__attribute__((unused)) const auto tau_f = Tau<float>;
	__attribute__((unused)) const auto tau_d = Tau<double>;
} // ns anon
		
		
} // ns math
} // ns stardazed
