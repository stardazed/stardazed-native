// ------------------------------------------------------------------
// physics::GlobalTime - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_PHYSICS_GLOBALTIME_H
#define SD_PHYSICS_GLOBALTIME_H

#include "system/Config.hpp"
#include "system/Time.hpp"
#include "physics/Units.hpp"

namespace stardazed {
namespace physics {


class GlobalTime {
	double time_;

public:
	GlobalTime()
	: time_(time::now().count())
	{}
	explicit constexpr GlobalTime(double timeStamp)
	: time_(timeStamp)
	{}
	constexpr GlobalTime(const GlobalTime& other)
	: time_(other.time_)
	{}

	// immutable
	GlobalTime& operator =(const GlobalTime&) = delete;

	// direct double access
	constexpr double value() const { return time_; }

	// export as float-precision Time
	explicit constexpr operator Time() const {
		return Time{ static_cast<float>(time_) };
	}
};


constexpr GlobalTime operator +(GlobalTime a, GlobalTime b) {
	return GlobalTime{ a.value() + b.value() };
}

constexpr GlobalTime operator -(GlobalTime a, GlobalTime b) {
	return GlobalTime{ a.value() - b.value() };
}


} // ns physics
} // ns stardazed

#endif
