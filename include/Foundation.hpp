// ------------------------------------------------------------------
// math::Foundation - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_MATH_FOUNDATION_H
#define SD_MATH_FOUNDATION_H

#include "ConceptTraits.hpp"

#include <type_traits>
#include <cmath>


namespace stardazed {
namespace math {

// ---- Constants

template <typename T>
constexpr const T pi = T{ 3.14159265358979323846 };

template <typename T>
constexpr const T tau = pi<T> * T{2};


namespace detail {

	// scalar wrapper base-type to allow for distinctly typed numerical objects
	template <typename Tag, typename T>
	struct Scalar : public FullyComparableTrait<Scalar<Tag, T>> {
		using ValueType = T;

		T value {0};
		
		constexpr Scalar() = default;
		
		template <typename S>
		constexpr explicit Scalar(S value, std::enable_if_t<std::is_convertible<S, T>::value>* = nullptr)
		: value{T(value)} {}
		
		constexpr bool operator==(const Scalar<Tag, T>& other) const { return value == other.value; }
		constexpr bool operator<(const Scalar<Tag, T>& other) const { return value < other.value; }
		
		Scalar<Tag, T>& operator+=(const Scalar<Tag, T>& other) {
			value += other.value;
			return *this;
		}
		Scalar<Tag, T> operator+(const Scalar<Tag, T>& other) const {
			auto result = *this;
			return result += other;
		}
	};

	// unit tag classes for Scalar
	struct RadiansTag {};
	struct DegreesTag {};
	
} // ns detail


// ---- Numeric units

using Radians = detail::Scalar<detail::RadiansTag, float>;
using Degrees = detail::Scalar<detail::DegreesTag, float>;


// ---- Literals

constexpr Radians operator ""_rad(long double val) {
	return Radians{ static_cast<Radians::ValueType>(val) };
}

constexpr Degrees operator ""_deg(long double val) {
	return Degrees{ static_cast<Degrees::ValueType>(val) };
}

constexpr Degrees operator ""_deg(unsigned long long val) {
	return Degrees{ static_cast<Degrees::ValueType>(val) };
}


// ---- Conversion

template <typename T>
Radians asRadians(T);
template<>
constexpr Radians asRadians(Radians rad) { return rad; }
template<>
constexpr Radians asRadians(Degrees deg) { return Radians{ deg.value * pi<Degrees::ValueType> / Degrees::ValueType{180} }; }

template <typename T>
Degrees asDegrees(T);
template<>
constexpr Degrees asDegrees(Degrees deg) { return deg; }
template<>
constexpr Degrees asDegrees(Radians rad) { return Degrees{ rad.value * Radians::ValueType{180} / pi<Radians::ValueType> }; };


// ---- Angle

class Angle  {
	Radians theta{};

public:
	constexpr Angle() = default;

	// not explicit as the unit type already is explicit (Degrees or Radians)
	// and setting an angle to 45 degrees or π radians feels natural.
	template <typename Unit>
	constexpr Angle(const Unit& u) : theta{asRadians(u)} {}

	template <typename Unit>
	Angle& operator=(const Unit& u) {
		theta = asRadians(u);
		return *this;
	}

	constexpr Radians rad() const { return theta; }
	constexpr Degrees deg() const { return asDegrees(theta); }
};


} // ns math
} // ns stardazed

#endif
