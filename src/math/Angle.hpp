// ------------------------------------------------------------------
// math::Angle - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_MATH_ANGLE_H
#define SD_MATH_ANGLE_H

#include "util/ConceptTraits.hpp"
#include "math/Constants.hpp"

#include <type_traits>
#include <cmath>


namespace stardazed {
namespace math {

namespace detail {

	// angle size scalar wrapper base-type
	template <typename Tag, typename T>
	class AngleSize : public FullyComparableTrait<AngleSize<Tag, T>> {
		T value_ {0};

	public:
		using ValueType = T;
		
		constexpr AngleSize() = default;
		
		template <typename S>
		constexpr explicit AngleSize(S value, std::enable_if_t<std::is_convertible<S, T>::value>* = nullptr)
		: value_{T(value)} {}
		
		constexpr const T val() const { return value_; }
		
		constexpr bool operator==(const AngleSize<Tag, T>& other) const { return value_ == other.value_; }
		constexpr bool operator<(const AngleSize<Tag, T>& other) const { return value_ < other.value_; }

		// angles can be added to or subtracted from eachother
		constexpr const AngleSize<Tag, T>& operator+=(const AngleSize<Tag, T>& other) {
			value_ += other.value_;
			return *this;
		}
		constexpr const AngleSize<Tag, T> operator+(const AngleSize<Tag, T>& other) const {
			auto result = *this;
			return result += other;
		}
		constexpr const AngleSize<Tag, T>& operator-=(const AngleSize<Tag, T>& other) {
			value_ -= other.value_;
			return *this;
		}
		constexpr const AngleSize<Tag, T> operator-(const AngleSize<Tag, T>& other) const {
			auto result = *this;
			return result -= other;
		}
		constexpr const AngleSize<Tag, T> operator-() const {
			return AngleSize<Tag, T>{ -value_ };
		}

		// angles can only be scaled by a normal scalar
		// scalar / angle and scalar % angle are not defined
		constexpr const AngleSize<Tag, T>& operator *=(T scalar) {
			value_ *= scalar;
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
			value_ /= scalar;
			return *this;
		}
		constexpr const AngleSize<Tag, T> operator /(T scalar) const {
			auto result = *this;
			return result /= scalar;
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
constexpr Radians asRadians(Degrees deg) { return Radians{ deg.val() * Pi<Degrees::ValueType> / Degrees::ValueType{180} }; }

template <typename T>
Degrees asDegrees(T);
template<>
constexpr Degrees asDegrees(Degrees deg) { return deg; }
template<>
constexpr Degrees asDegrees(Radians rad) { return Degrees{ rad.val() * Radians::ValueType{180} / Pi<Radians::ValueType> }; };


// ---- Angle abstracts away radians and degrees and allows —explicit— mixing of the two
// converting everything to its internal format

class Angle  {
	Radians theta{};

public:
	constexpr Angle() = default;

	// not explicit as the unit type already is explicit (Degrees or Radians)
	// and setting an angle to 45 degrees or π radians feels natural.
	constexpr Angle(const Radians& u) : theta{u} {}
	constexpr Angle(const Degrees& u) : theta{asRadians(u)} {}
	
	template Angle(const Radians::ValueType&) = delete; // explicitly disallow direct init from underlying type

	template <typename Unit>
	Angle& operator=(const Unit& u) {
		theta = asRadians(u);
		return *this;
	}

	constexpr Radians rad() const { return theta; }
	constexpr Degrees deg() const { return asDegrees(theta); }
};


// ---- Specializations of trig functions to allow for idiomatic usage

inline float sin(Degrees d) { return std::sinf(asRadians(d).val()); }
inline float cos(Degrees d) { return std::cosf(asRadians(d).val()); }
inline float tan(Degrees d) { return std::tanf(asRadians(d).val()); }

inline float sin(Radians r) { return std::sinf(r.val()); }
inline float cos(Radians r) { return std::cosf(r.val()); }
inline float tan(Radians r) { return std::tanf(r.val()); }

inline float sin(Angle a) { return std::sinf(a.rad().val()); }
inline float cos(Angle a) { return std::cosf(a.rad().val()); }
inline float tan(Angle a) { return std::tanf(a.rad().val()); }


// potentially accelerated two-fer methods (specialized per platform)

inline void sincos(Degrees d, float& s, float& c) { __sincosf(asRadians(d).val(), &s, &c); }
inline void sincos(Radians r, float& s, float& c) { __sincosf(r.val(), &s, &c); }
inline void sincos(Angle a, float& s, float& c) { __sincosf(a.rad().val(), &s, &c); }


} // ns math
} // ns stardazed

#endif
