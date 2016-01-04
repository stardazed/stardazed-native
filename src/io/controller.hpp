// ------------------------------------------------------------------
// io::controller - stardazed
// (c) 2016 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_IO_CONTROLLER_H
#define SD_IO_CONTROLLER_H

#include "system/Config.hpp"
#include "io/input.hpp"

namespace stardazed {
namespace io {


struct ControllerState { // 4 + (3 * 8) + (10 * 2) + 8 = 56 bytes
	bool16 isConnected, isAnalog;
	Stick leftStick, rightStick;
	DirectionalPad dPad;
	ButtonState A, B, X, Y, leftShoulder, rightShoulder;
	ButtonState leftThumb, rightThumb;
	ButtonState select, start;
	float leftTrigger, rightTrigger;
};


struct Controller {
	virtual ~Controller() = default;

	virtual uint count() = 0;
	virtual bool enabled(uint index) = 0;
	virtual ControllerState* state(uint index) = 0;
};


Controller& controller();


} // ns io
} // ns stardazed

#endif
