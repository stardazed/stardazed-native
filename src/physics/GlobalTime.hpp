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
	constexpr GlobalTime(const time::Duration& duration)
	: time_(duration.count())
	{}

	// direct double access
	constexpr operator double() const { return time_; }
	
	// friends
	friend GlobalTime& operator +=(GlobalTime& into, const GlobalTime add) {
		into.time_ += add.time_;
		return into;
	}

	friend GlobalTime& operator -=(GlobalTime& into, const GlobalTime add) {
		into.time_ -= add.time_;
		return into;
	}
};


constexpr GlobalTime operator +(GlobalTime a, GlobalTime b) {
	return GlobalTime{ a + b };
}

constexpr GlobalTime operator -(GlobalTime a, GlobalTime b) {
	return GlobalTime{ a - b };
}


} // ns physics
} // ns stardazed

#endif
