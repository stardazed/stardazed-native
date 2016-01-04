// ------------------------------------------------------------------
// io::mac_controller - stardazed
// (c) 2016 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_IO_MAC_CONTROLLER_H
#define SD_IO_MAC_CONTROLLER_H

#include "system/Config.hpp"
#include "io/controller.hpp"

#include <vector>

#include <IOKit/hid/IOHIDLib.h>


namespace stardazed {
namespace io {


class MacController;

struct ControllerDriverContext {
	ControllerState controller;
	MacController* context;
};


struct ControllerDriver {
	virtual ~ControllerDriver() = default;

	virtual bool supportsDevice(IOHIDDeviceRef, int vendorID, int productID) = 0;
	virtual IOHIDValueCallback callbackForDevice(IOHIDDeviceRef, int vendorID, int productID) = 0;
};


class MacController : public Controller {
	std::vector<ControllerDriver*> controllerDrivers_;
	std::vector<ControllerDriverContext> controllers_;
	
	using DriverIter = decltype(controllerDrivers_)::const_iterator;
	
	IOHIDManagerRef hidManager_;
	
public:
	MacController();

	uint count() override;
	bool enabled(uint index) override;
	ControllerState* state(uint index) override;

	ControllerDriverContext& createController();
	
	DriverIter driversBegin() const { return controllerDrivers_.cbegin(); }
	DriverIter driversEnd() const { return controllerDrivers_.cend(); }
};


} // ns io
} // ns stardazed

#endif
