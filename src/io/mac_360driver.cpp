// ------------------------------------------------------------------
// io::mac_360driver - stardazed
// (c) 2016 by Arthur Langereis
// ------------------------------------------------------------------

#include "io/mac_360driver.hpp"
#include "math/Algorithm.hpp"

namespace stardazed {
namespace io {


// taken from MSDN, defined in XInput.h
static const int X360LeftThumbDeadZone  = 7849;
static const int X360RightThumbDeadZone = 8689;


static void X360ValueCallback(void* context, IOReturn, void*, IOHIDValueRef value) {
	// -- acquire runtime context
	auto controllerCtx = static_cast<ControllerDriverContext*>(context);
	auto& controller = controllerCtx->controller;
	// auto devicesCtx = controllerCtx->devicesContext;

	IOHIDElementRef element = IOHIDValueGetElement(value);
	if (CFGetTypeID(element) != IOHIDElementGetTypeID()) {
		return;
	}
	
	int usage = IOHIDElementGetUsage(element);
	CFIndex elementValue = IOHIDValueGetIntegerValue(value);
	
	if (usage == 50) {
		// Left Trigger
		float normValue = (float)elementValue / 255.f;
		controller.leftTrigger = normValue;
	}
	else if (usage == 53) {
		// Right Trigger
		float normValue = (float)elementValue / 255.f;
		controller.rightTrigger = normValue;
	}
	else if (usage > 47) {
		// Thumb Sticks
		float normValue = elementValue;
		float deadZone = usage < 50 ? X360LeftThumbDeadZone : X360RightThumbDeadZone;
		
		if (normValue < 0) {
			normValue = math::min(0.0f, normValue + deadZone) / (32768.f - deadZone);
		}
		else {
			normValue = math::max(0.0f, normValue - deadZone) / (32767.f - deadZone);
		}
		
		switch (usage) {
			case 48: controller.leftStick.posX  = normValue; break;
			case 49: controller.leftStick.posY  = normValue; break;
			case 51: controller.rightStick.posX = normValue; break;
			case 52: controller.rightStick.posY = normValue; break;
		}
	}
	else {
		// Buttons
		ButtonState* button {};
		
		switch (usage) {
			case  1: button = &controller.A; break;
			case  2: button = &controller.B; break;
			case  3: button = &controller.X; break;
			case  4: button = &controller.Y; break;
			case  5: button = &controller.leftShoulder; break;
			case  6: button = &controller.rightShoulder; break;
			case  7: button = &controller.leftThumb; break;
			case  8: button = &controller.rightThumb; break;
			case  9: button = &controller.start; break;
			case 10: button = &controller.select; break;
			case 11: break; // home button, not supported
			case 12: button = &controller.dPad.up; break;
			case 13: button = &controller.dPad.down; break;
			case 14: button = &controller.dPad.left; break;
			case 15: button = &controller.dPad.right; break;
			default: break;
		}
		
		if (button) {
			++button->halfTransitionCount;
			button->endedDown = elementValue == 1;
		}
	}
}


bool X360ControllerDriver::supportsDevice(IOHIDDeviceRef, int vendorID, int productID) {
	return vendorID == 1118 && productID == 654;
}


IOHIDValueCallback X360ControllerDriver::callbackForDevice(IOHIDDeviceRef, int, int) {
	return X360ValueCallback;
}

	
} // ns io
} // ns stardazed
