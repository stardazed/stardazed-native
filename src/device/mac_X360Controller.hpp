// ------------------------------------------------------------------
// device::mac_X360Controller - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_DEVICE_MAC_X360CONTROLLER_H
#define SD_DEVICE_MAC_X360CONTROLLER_H

#include "system/Config.hpp"
#include "device/Controller.hpp"

#include <IOKit/hid/IOHIDBase.h>

namespace stardazed {
namespace device {


class X360ControllerDriver : public ControllerDriver {
public:
	bool supportsDevice(IOHIDDeviceRef, int, int) override;
	IOHIDValueCallback callbackForDevice(IOHIDDeviceRef, int vendorID, int productID) override;
};


} // ns device
} // ns stardazed


#endif
