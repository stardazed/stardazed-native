// ------------------------------------------------------------------
// device::Keyboard - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_DEVICE_MAC_X360CONTROLLER_H
#define SD_DEVICE_MAC_X360CONTROLLER_H

#include "system/Config.hpp"
#include "device/Controller.hpp"

#include <IOKit/hid/IOHIDBase.h>

namespace stardazed {
namespace device {

static const int X360LeftThumbDeadZone = 7849;
static const int X360RightThumbDeadZone = 8689;


class X360ControllerHIDClient {
public:
	static bool supportsHIDDevice(IOHIDDeviceRef);
	
};


} // ns device
} // ns stardazed


#endif
