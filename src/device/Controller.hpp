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


struct Stick { // 16 bytes
	float posX, posY;
	Button left, right, up, down;
};


struct Controller { // 4 + (3 * 16) + (8 * 2) + 8 = 68 bytes
	bool16 isConnected, isAnalog;
	Stick leftStick, rightStick, dPad;
	Button A, B, X, Y, L1, R1;
	Button select, start;
	float L2, R2;
};

	
	
} // ns device
} // ns stardazed

#endif
