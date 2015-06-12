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


//  ___ _____   __      _
// / __|_ _\ \ / /__ __| |_ ___ _ _
// \__ \| | \ V / -_) _|  _/ _ \ '_|
// |___/___| \_/\___\__|\__\___/_|
//

template <int KG, int M, int S, size32 N>
struct SIVector;


template <int KG, int M, int S, size32 N>
constexpr SIVector<KG, M, S, N>
operator *(float scalar, const SIVector<KG, M, S, N>& ua) {
	return SIVector<KG, M, S, N>{ scalar * ua.value };
}


template <int KG, int M, int S, size32 N>
constexpr SIVector<KG, M, S, N>
operator *(const SIVector<KG, M, S, N>& ua, float scalar) {
	return SIVector<KG, M, S, N>{ ua.value * scalar };
}


template <int KGA, int MA, int SA, int KGB, int MB, int SB, size32 N>
constexpr SIVector<KGA + KGB, MA + MB, SA + SB, N>
operator *(const SIVector<KGA, MA, SA, N>& ua, const SIVector<KGB, MB, SB, N>& ub) {
	return SIVector<KGA + KGB, MA + MB, SA + SB, N>{ ua.value * ub.value };
}


template <int KG, int M, int S, size32 N>
constexpr SIVector<-KG, -M, -S, N>
operator /(float scalar, const SIVector<KG, M, S, N>& ua) {
	return SIVector<-KG, -M, -S, N>{ scalar / ua.value };
}


template <int KG, int M, int S, size32 N>
constexpr SIVector<KG, M, S, N>
operator /(const SIVector<KG, M, S, N>& ua, float scalar) {
	return SIVector<KG, M, S, N>{ ua.value / scalar };
}


template <int KGA, int MA, int SA, int KGB, int MB, int SB, size32 N>
constexpr SIVector<KGA - KGB, MA - MB, SA - SB, N>
operator /(const SIVector<KGA, MA, SA, N>& ua, const SIVector<KGB, MB, SB, N>& ub) {
	return SIVector<KGA - KGB, MA - MB, SA - SB, N>{ ua.value / ub.value };
}


template <int KG, int M, int S, size32 N>
constexpr SIVector<KG, M, S, N>
operator +(const SIVector<KG, M, S, N>& ua, const SIVector<KG, M, S, N>& ub) {
	return SIVector<KG, M, S, N>{ ua.value + ub.value };
}


template <int KG, int M, int S, size32 N>
constexpr SIVector<KG, M, S, N>
operator -(const SIVector<KG, M, S, N>& ua, const SIVector<KG, M, S, N>& ub) {
	return SIVector<KG, M, S, N>{ ua.value - ub.value };
}


// -- modify-assign
// no *= or /= because it would modify the dimension of the assigned value

template <int KG, int M, int S, size32 N>
constexpr SIVector<KG, M, S, N>&
operator +=(SIVector<KG, M, S, N>& ua, const SIVector<KG, M, S, N>& ub) {
	ua.value += ub.value;
	return ua;
}


template <int KG, int M, int S, size32 N>
constexpr SIVector<KG, M, S, N>&
operator +=(SIVector<KG, M, S, N>& ua, float scalar) {
	ua.value += scalar;
	return ua;
}


template <int KG, int M, int S, size32 N>
constexpr SIVector<KG, M, S, N>&
operator -=(SIVector<KG, M, S, N>& ua, const SIVector<KG, M, S, N>& ub) {
	ua.value -= ub.value;
	return ua;
}


template <int KG, int M, int S, size32 N>
constexpr SIVector<KG, M, S, N>&
operator -=(SIVector<KG, M, S, N>& ua, float scalar) {
	ua.value -= scalar;
	return ua;
}


//  ___ _____   __    _
// / __|_ _\ \ / /_ _| |_  _ ___
// \__ \| | \ V / _` | | || / -_)
// |___/___| \_/\__,_|_|\_,_\___|
// 								  

template <int KG, int M, int S>
struct SIVector<KG, M, S, 1> {
	float value;
	
	constexpr SIVector() = default;
	explicit constexpr SIVector(float x) : value{x} {}
	constexpr SIVector(const SIVector<KG, M, S, 1>& other) : value{other.value} {}
};

template <int KG, int M, int S>
using SIValue = SIVector<KG, M, S, 1>;


//  ___ _____   __    _          ___ 
// / __|_ _\ \ / /_ _| |_  _ ___|_  )
// \__ \| | \ V / _` | | || / -_)/ / 
// |___/___| \_/\__,_|_|\_,_\___/___|
// 								  

template <int KG, int M, int S>
struct SIVector<KG, M, S, 2> {
	math::Vec2 value;
	using ValueType = SIValue<KG, M, S>;
	
	constexpr SIVector() = default;
	constexpr SIVector(math::Vec2 val) : value(val) {}
	constexpr SIVector(float x, float y) : value{x, y} {}
	constexpr SIVector(ValueType x, ValueType y) : value{x.value, y.value} {}


	constexpr ValueType x() const { return value.x; }
	constexpr ValueType y() const { return value.y; }
};

template <int KG, int M, int S>
using SIValue2 = SIVector<KG, M, S, 2>;


//  ___ _____   __    _          ____
// / __|_ _\ \ / /_ _| |_  _ ___|__ /
// \__ \| | \ V / _` | | || / -_)|_ \
// |___/___| \_/\__,_|_|\_,_\___|___/
// 								  

template <int KG, int M, int S>
struct SIVector<KG, M, S, 3> {
	math::Vec3 value;
	using ValueType = SIValue<KG, M, S>;
	
	constexpr SIVector() = default;
	constexpr SIVector(math::Vec3 val) : value(val) {}
	constexpr SIVector(float x, float y, float z) : value{x, y, z} {}
	constexpr SIVector(ValueType x, ValueType y, ValueType z) : value{x.value, y.value, z.value} {}

	constexpr ValueType x() const { return value.x; }
	constexpr ValueType y() const { return value.y; }
	constexpr ValueType z() const { return value.z; }
};

template <int KG, int M, int S>
using SIValue3 = SIVector<KG, M, S, 3>;


//  ___ _____   __    _          _ _  
// / __|_ _\ \ / /_ _| |_  _ ___| | | 
// \__ \| | \ V / _` | | || / -_)_  _|
// |___/___| \_/\__,_|_|\_,_\___| |_| 
// 								   

template <int KG, int M, int S>
struct SIVector<KG, M, S, 4> {
	math::Vec4 value;
	using ValueType = SIValue<KG, M, S>;
	using Value3Type = SIValue3<KG, M, S>;
	
	constexpr SIVector() = default;
	constexpr SIVector(Value3Type val3) : value(val3.value, 0) {}
	constexpr SIVector(math::Vec4 val) : value(val) {}
	constexpr SIVector(float x, float y, float z, float w = 0) : value{x, y, z, w} {}
	constexpr SIVector(ValueType x, ValueType y, ValueType z, ValueType w = 0) : value{x.value, y.value, z.value, w.value} {}
	
	constexpr ValueType x() const { return value.x; }
	constexpr ValueType y() const { return value.y; }
	constexpr ValueType z() const { return value.z; }
	constexpr ValueType w() const { return value.w; }
	
	constexpr Value3Type xyz() const { return { value.xyz }; }
};

template <int KG, int M, int S>
using SIValue4 = SIVector<KG, M, S, 4>;



//          _      _
//  ____ __| |__ _| |_
// (_-< '_ \ / _` |  _|
// /__/ .__/_\__,_|\__|
//    |_|

template <int KG, int M, int S>
constexpr SIVector<KG, M, S, 2>
splat2(const SIValue<KG, M, S>& v) {
	return { v, v };
}

template <int KG, int M, int S>
constexpr SIVector<KG, M, S, 3>
splat3(const SIValue<KG, M, S>& v) {
	return { v, v, v };
}

template <int KG, int M, int S>
constexpr SIVector<KG, M, S, 4>
splat4(const SIValue<KG, M, S>& v) {
	return { v, v, v, v };
}



//  ___ ___   _   _      _ _
// / __|_ _| | | | |_ _ (_) |_ ___
// \__ \| |  | |_| | ' \| |  _(_-<
// |___/___|  \___/|_||_|_|\__/__/
//

/*
	Mass           kg
	Position       m            x(yz)
	Time           s            t

	Velocity       m s-1        a
	Acceleration   m s-2        v

	Momentum       kg m s-1     kg v     N s
	Force          kg m s-2     kg a     N(ewton)
*/

using Mass = SIValue<1, 0, 0>;
using Time = SIValue<0, 0, 1>;

using Position  = SIValue <0, 1, 0>;
using Position2 = SIValue2<0, 1, 0>;
using Position3 = SIValue3<0, 1, 0>;
using Position4 = SIValue4<0, 1, 0>;

using Velocity  = SIValue <0, 1, -1>;
using Velocity2 = SIValue2<0, 1, -1>;
using Velocity3 = SIValue3<0, 1, -1>;
using Velocity4 = SIValue4<0, 1, -1>;

using Acceleration  = SIValue <0, 1, -2>;
using Acceleration2 = SIValue2<0, 1, -2>;
using Acceleration3 = SIValue3<0, 1, -2>;
using Acceleration4 = SIValue4<0, 1, -2>;

using Momentum  = SIValue <1, 1, -1>;
using Momentum2 = SIValue2<1, 1, -1>;
using Momentum3 = SIValue3<1, 1, -1>;
using Momentum4 = SIValue4<1, 1, -1>;

using Force  = SIValue <1, 1, -2>;
using Force2 = SIValue2<1, 1, -2>;
using Force3 = SIValue3<1, 1, -2>;
using Force4 = SIValue4<1, 1, -2>;


//  _  _     _
// | || |___| |_ __  ___ _ _ ___
// | __ / -_) | '_ \/ -_) '_(_-<
// |_||_\___|_| .__/\___|_| /__/
//            |_|

constexpr Position3
operator *(const math::Quat& quat, const Position3& pos) {
	return { quat * pos.value };
}


template <int KG, int M, int S, size32 N>
std::string toString(const SIVector<KG, M, S, N>& siv) {
	return sd::toString(siv.value);
}


//   ___             _            _
//  / __|___ _ _  __| |_ __ _ _ _| |_ ___
// | (__/ _ \ ' \(_-<  _/ _` | ' \  _(_-<
//  \___\___/_|\_/__/\__\__,_|_|\_\__/__/
//

constexpr Acceleration3 earthGravity() { return { 0, -9.80665, 0 }; }
constexpr Acceleration3 moonGravity() { return { 0, -1.62519, 0 }; }


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

}


} // ns physics
} // ns stardazed

#endif
