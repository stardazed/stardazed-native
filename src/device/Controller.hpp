// ------------------------------------------------------------------
// device::Controller - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_DEVICE_CONTROLLER_H
#define SD_DEVICE_CONTROLLER_H

#include "system/Config.hpp"

namespace stardazed {
namespace device {
	

struct Button { // 2 bytes
	uint8_t halfTransitionCount;
	bool8 endedDown;
};


struct Stick { // 8 bytes
	float posX, posY;
};


struct DirectionalPad { // 8 bytes
	Button left, right, up, down;
};


struct Controller { // 4 + (3 * 8) + (10 * 2) + 8 = 56 bytes
	bool16 isConnected, isAnalog;
	Stick leftStick, rightStick;
	DirectionalPad dPad;
	Button A, B, X, Y, leftShoulder, rightShoulder;
	Button leftThumb, rightThumb;
	Button select, start;
	float leftTrigger, rightTrigger;
};


} // ns device
} // ns stardazed

#endif
