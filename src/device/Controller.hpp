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
	uint8 halfTransitionCount;
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


inline Stick stickFromDPad(const DirectionalPad& dPad) {
	Stick stick {};
	
	if (dPad.left.endedDown)
		stick.posX = -1;
	else if (dPad.right.endedDown)
		stick.posX =  1;

	if (dPad.up.endedDown)
		stick.posY = -1;
	else if (dPad.down.endedDown)
		stick.posY =  1;
	
	return stick;
}


} // ns device
} // ns stardazed

#endif
