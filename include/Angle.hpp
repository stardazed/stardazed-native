// ------------------------------------------------------------------
// math::Angle - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_MATH_ANGLE_H
#define SD_MATH_ANGLE_H

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

	// angle size scalar wrapper base-type
	template <typename Tag, typename T>
	class AngleSize : public FullyComparableTrait<AngleSize<Tag, T>> {
		T value {0};

	public:
		using ValueType = T;
		
		constexpr AngleSize() = default;
		
		template <typename S>
		constexpr explicit AngleSize(S value, std::enable_if_t<std::is_convertible<S, T>::value>* = nullptr)
		: value{T(value)} {}
		
		constexpr const T val() const { return value; }
		
		constexpr bool operator==(const AngleSize<Tag, T>& other) const { return value == other.value; }
		constexpr bool operator<(const AngleSize<Tag, T>& other) const { return value < other.value; }

		// angles can be added to or subtracted from eachother
		constexpr const AngleSize<Tag, T>& operator+=(const AngleSize<Tag, T>& other) {
			value += other.value;
			return *this;
		}
		constexpr const AngleSize<Tag, T> operator+(const AngleSize<Tag, T>& other) const {
			auto result = *this;
			return result += other;
		}
		constexpr const AngleSize<Tag, T>& operator-=(const AngleSize<Tag, T>& other) {
			value -= other.value;
			return *this;
		}
		constexpr const AngleSize<Tag, T> operator-(const AngleSize<Tag, T>& other) const {
			auto result = *this;
			return result -= other;
		}
		constexpr const AngleSize<Tag, T> operator-() const {
			return AngleSize<Tag, T>{ -value };
		}

		// angles can only be scaled by a normal scalar
		// scalar / angle and scalar % angle are not defined
		constexpr const AngleSize<Tag, T>& operator *=(T scalar) {
			value *= scalar;
			return *this;
		}
		constexpr const AngleSize<Tag, T> operator *(T scalar) const {
			auto result = *this;
			return result *= scalar;
		}
		friend constexpr const AngleSize<Tag, T> operator *(T scalar, const AngleSize<Tag, T>& as) {
			// multiplication is commutative
			return as * scalar;
		}
		constexpr const AngleSize<Tag, T>& operator /=(T scalar) {
			value *= scalar;
			return *this;
		}
		constexpr const AngleSize<Tag, T> operator /(T scalar) const {
			auto result = *this;
			return result *= scalar;
		}
		constexpr const AngleSize<Tag, T>& operator %=(T scalar) {
			value %= scalar;
			return *this;
		}
		constexpr const AngleSize<Tag, T> operator %(T scalar) const {
			auto result = *this;
			return result %= scalar;
		}
	};

	// unit tag classes for Scalar
	struct RadiansTag {};
	struct DegreesTag {};
	
} // ns detail


// ---- Numeric units

using Radians = detail::AngleSize<detail::RadiansTag, float>;
using Degrees = detail::AngleSize<detail::DegreesTag, float>;


// ---- Literals

constexpr Radians operator ""_rad(long double val) {
	return Radians{ static_cast<Radians::ValueType>(val) };
}

constexpr Radians operator ""_rad(unsigned long long val) {
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
constexpr Radians asRadians(Degrees deg) { return Radians{ deg.val() * pi<Degrees::ValueType> / Degrees::ValueType{180} }; }

template <typename T>
Degrees asDegrees(T);
template<>
constexpr Degrees asDegrees(Degrees deg) { return deg; }
template<>
constexpr Degrees asDegrees(Radians rad) { return Degrees{ rad.val() * Radians::ValueType{180} / pi<Radians::ValueType> }; };


// ---- Angle abstracts away radians and degrees and allows —explicit— mixing of the two
// converting everything to its internal format

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
