// ------------------------------------------------------------------
// io::mac_360driver - stardazed
// (c) 2016 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_IO_MAC_360DRIVER_H
#define SD_IO_MAC_360DRIVER_H

#include "system/Config.hpp"
#include "io/mac_controller.hpp"

namespace stardazed {
namespace io {


class X360ControllerDriver : public ControllerDriver {
public:
	bool supportsDevice(IOHIDDeviceRef, int, int) override;
	IOHIDValueCallback callbackForDevice(IOHIDDeviceRef, int vendorID, int productID) override;
};


} // ns io
} // ns stardazed


#endif
