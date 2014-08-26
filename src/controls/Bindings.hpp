// ------------------------------------------------------------------
// controls::Bindings - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_CONTROLS_BINDINGS_H
#define SD_CONTROLS_BINDINGS_H

#include "math/Vector.hpp"
#include "input/Keyboard.hpp"

namespace stardazed {
namespace controls {


enum class DeviceID {
	None,
	Keyboard,
	Mouse,
	Joystick0,
	Joystick1,
	Joystick2,
	Joystick3
};


class Button {
	float value_ = 0;
	DeviceID deviceID_;
	union {
		event::Key key_;
		int controlID_;
	};

public:
	void update();
	float value() const { return value_; }; // 0..1
};


class Axis {
	float value_ = 0;
	DeviceID deviceID_;
	union {
		struct { event::Key keyA, keyB; };
		int controlID;
	};

public:
	void update();
	float value() const { return value_; }; // -1..1
};


class Axis2D {
	Vec2 value2D_ = {0,0};
	Axis axisX_;
	Axis axisY_;

public:
	Axis2D(Axis x, Axis y);
	const Axis& axisX() const { return axisX_; }
	const Axis& axisY() const { return axisY_; }
	
	void update(input::InputContext& input) {
		axisX_.update(input);
		axisY_.update(input);

		value2D_.x = axisX_.value();
		value2D_.y = axisY_.value();
	}
	
	Vec2 value2D() const { return value2D_; }
};


template <typename B>
class DoubleBinding {
	float value_ = 0;
	B primary;
	B secondary;
public:
	void update(input::InputContext& input) {
		primary.update(input);
		auto v = primary.value();

		if (v == 0.0f) { // give or take a threshold
			secondary.update(input);
			v = secondary.value();
		}

		value_ = v;
	}
	float value() const { return value_; };
};



} // ns controls
} // ns stardazed

#endif
