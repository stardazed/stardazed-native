// ------------------------------------------------------------------
// physics::Units - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_PHYSICS_UNITS_H
#define SD_PHYSICS_UNITS_H

#include "system/Config.hpp"
#include "util/StringFormat.hpp"
#include "math/Vector.hpp"
#include "math/Quaternion.hpp"

namespace stardazed {
namespace physics {


//  ___ _____   __    _
// / __|_ _\ \ / /_ _| |_  _ ___
// \__ \| | \ V / _` | | || / -_)
// |___/___| \_/\__,_|_|\_,_\___|
// 								  

template <int KG, int M, int S, int RAD>
struct SIValue {
	float data {0};
	
	constexpr SIValue() = default;
	constexpr SIValue(float x) : data{x} {}
	
	constexpr explicit operator float() const { return data; }
};

#define SIVal SIValue<KG, M, S, RAD>
#define SIVal_A SIValue<KGA, MA, SA, RADA>
#define SIVal_B SIValue<KGB, MB, SB, RADB>
#define SIVal_AxB(OP) SIValue<KGA OP KGB, MA OP MB, SA OP SB, RADA OP RADB>

template <int KG, int M, int S, int RAD>
constexpr SIVal
operator -(const SIVal& a) {
	return -float(a);
}

template <int KG, int M, int S, int RAD>
constexpr SIVal
operator +(const SIVal& a, const SIVal& b) {
	return float(a) + float(b);
}

template <int KG, int M, int S, int RAD>
constexpr SIVal
operator -(const SIVal& a, const SIVal& b) {
	return float(a) - float(b);
}

template <int KGA, int MA, int SA, int RADA, int KGB, int MB, int SB, int RADB>
constexpr SIVal_AxB(+)
operator *(const SIVal_A& a, const SIVal_B& b) {
	return float(a) * float(b);
}

template <int KG, int M, int S, int RAD>
constexpr SIVal
operator *(const SIVal& a, float f) {
	return float(a) * f;
}

template <int KG, int M, int S, int RAD>
constexpr SIVal
operator *(float f, const SIVal& a) {
	return f * float(a);
}

template <int KGA, int MA, int SA, int RADA, int KGB, int MB, int SB, int RADB>
constexpr SIVal_AxB(-)
operator /(const SIVal_A& a, const SIVal_B& b) {
	return float(a) / float(b);
}

template <int KG, int M, int S, int RAD>
constexpr SIVal
operator /(const SIVal& a, float f) {
	return float(a) / f;
}

template <int KG, int M, int S, int RAD>
constexpr SIVal
operator /(float f, const SIVal& a) {
	return f / float(a);
}
	

#undef SIVal
#undef SIVal_A
#undef SIVal_B
#undef SIVal_AxB


//  ___ ___   _   _      _ _
// / __|_ _| | | | |_ _ (_) |_ ___
// \__ \| |  | |_| | ' \| |  _(_-<
// |___/___|  \___/|_||_|_|\__/__/
//

#define SI_UNIT(name, KG, M, S, RAD) \
using name = SIValue<KG, M, S, RAD>; \
using Inverse##name = SIValue<-KG, -M, -S, -RAD>; \


#define SI_UNIT_VEC(name, KG, M, S, RAD) \
using name = SIValue<KG, M, S, RAD>; \
using name##2 = math::Vector<2, name>; \
using name##3 = math::Vector<3, name>; \
using name##4 = math::Vector<4, name>; \
using Inverse##name = SIValue<-KG, -M, -S, -RAD>; \
using Inverse##name##2 = math::Vector<2, Inverse##name>; \
using Inverse##name##3 = math::Vector<3, Inverse##name>; \
using Inverse##name##4 = math::Vector<4, Inverse##name>;

/*
	Mass           kg
	Position       m                     x(yz)
	Time           s                     t
	Angle          rad                   θ

	Velocity       m s-1                 a
	Acceleration   m s-2                 v
	Inertia        kg

	Momentum       kg m s-1     kg v     p          N s
	Force          kg m s-2     kg a     F          N(ewton)

	Ang Velocity   rad s-1               ⍵
	Ang Accel      rad s-2               ⍺
	Ang Inertia    kg m2 rad-2           I

	Ang Momentum   kg m2 rad-1 s-1       L
	Torque         kg m2 rad-1 s-2       T          N m
*/

SI_UNIT    (Mass,     1, 0, 0, 0)
SI_UNIT_VEC(Position, 0, 1, 0, 0)
SI_UNIT    (Time,     0, 0, 1, 0)
SI_UNIT_VEC(Angle,    0, 0, 0, 1)

SI_UNIT_VEC(Velocity,     0, 1, -1, 0)
SI_UNIT_VEC(Acceleration, 0, 1, -2, 0)
SI_UNIT_VEC(Inertia,      1, 0, 0, 0)

SI_UNIT_VEC(Momentum, 1, 1, -1, 0)
SI_UNIT_VEC(Force,    1, 1, -2, 0)

SI_UNIT_VEC(AngVelocity,   0, 0, -1, 1)
SI_UNIT_VEC(AngAccelation, 0, 0, -2, 1)
SI_UNIT_VEC(AngInertia,    1, 2, 0, -2)

SI_UNIT_VEC(AngMomentum,   1, 2, -1, -1)
SI_UNIT_VEC(Torque,        1, 2, -2, -1)

SI_UNIT    (Area,   0, 2, 0, 0)
SI_UNIT    (Volume, 0, 3, 0, 0)

SI_UNIT    (MassDensity, 1, -3, 0, 0);


#undef SI_UNIT
#undef SI_UNIT_VEC


//  _  _     _
// | || |___| |_ __  ___ _ _ ___
// | __ / -_) | '_ \/ -_) '_(_-<
// |_||_\___|_| .__/\___|_| /__/
//            |_|

constexpr Position3
operator *(const math::Quat& quat, const Position3& pos) {
	return { quat * pos };
}


inline physics::Angle toPhysicsAngle(math::Angle mathAngle) {
	return physics::Angle{ mathAngle.rad().val() };
}


inline math::Angle toMathAngle(physics::Angle physAngle) {
	return math::Radians{ physAngle.data };
}


//   ___             _            _
//  / __|___ _ _  __| |_ __ _ _ _| |_ ___
// | (__/ _ \ ' \(_-<  _/ _` | ' \  _(_-<
//  \___\___/_|\_/__/\__\__,_|_|\_\__/__/
//

constexpr Acceleration3 earthGravity() { return { 0, -9.80665, 0 }; }
constexpr Acceleration3 moonGravity() { return { 0, -1.62519, 0 }; }

constexpr MassDensity earthSeaLevelAirDensity() { return MassDensity{ 1.2250 }; } // at 15°C


//  _    _ _                _
// | |  (_) |_ ___ _ _ __ _| |___
// | |__| |  _/ -_) '_/ _` | (_-<
// |____|_|\__\___|_| \__,_|_/__/
//

inline namespace literals {

	constexpr Mass operator ""_kg(long double val) {
		return Mass{ static_cast<float>(val) };
	}

	constexpr Mass operator ""_kg(unsigned long long val) {
		return Mass{ static_cast<float>(val) };
	}
	
	constexpr Time operator ""_s(long double val) {
		return Time{ static_cast<float>(val) };
	}

	constexpr Time operator ""_s(unsigned long long val) {
		return Time{ static_cast<float>(val) };
	}
	
	constexpr Position operator ""_m(long double val) {
		return Position{ static_cast<float>(val) };
	}

	constexpr Position operator ""_m(unsigned long long val) {
		return Position{ static_cast<float>(val) };
	}

	constexpr Velocity operator ""_m_s(long double val) {
		return Velocity{ static_cast<float>(val) };
	}
	
	constexpr Velocity operator ""_m_s(unsigned long long val) {
		return Velocity{ static_cast<float>(val) };
	}

	constexpr Acceleration operator ""_m_s2(long double val) {
		return Acceleration{ static_cast<float>(val) };
	}
	
	constexpr Acceleration operator ""_m_s2(unsigned long long val) {
		return Acceleration{ static_cast<float>(val) };
	}

	constexpr Force operator ""_N(long double val) {
		return Force{ static_cast<float>(val) };
	}
	
	constexpr Force operator ""_N(unsigned long long val) {
		return Force{ static_cast<float>(val) };
	}

	constexpr Torque operator ""_Nm(long double val) {
		return Torque{ static_cast<float>(val) };
	}
	
	constexpr Torque operator ""_Nm(unsigned long long val) {
		return Torque{ static_cast<float>(val) };
	}

} // inline ns literals


} // ns physics
} // ns stardazed

#endif
