// ------------------------------------------------------------------
// device::mac_DevicesContext - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_DEVICE_MAC_DEVICESCONTEXT_H
#define SD_DEVICE_MAC_DEVICESCONTEXT_H

#include "system/Config.hpp"
#include "device/Keyboard.hpp"
#include "device/Controller.hpp"

#include <vector>
#include <array>

#include <IOKit/hid/IOHIDLib.h>

namespace stardazed {
namespace device {


struct KeyboardControllerConfig {
	Key dPadLeft, dPadRight, dPadUp, dPadDown;
	Key btnA, btnB, btnX, btnY;
	Key btnLeftShoulder, btnRightShoulder;
	Key btnSelect, btnStart;
	// Sticks and trigger are not supported by definition (analog)
	// Essentially, the keyboard is a SNES controller
};


class DevicesContext;

struct ControllerDriverContext {
	Controller controller;
	DevicesContext* devicesContext;
};


class ControllerDriver {
public:
	virtual ~ControllerDriver() {}
	virtual bool supportsDevice(IOHIDDeviceRef, int vendorID, int productID) = 0;
	virtual IOHIDValueCallback callbackForDevice(IOHIDDeviceRef, int vendorID, int productID) = 0;
};


class DevicesContext {
	Keyboard keyboard_;
	std::array<Key, 512> keyTransTable_;

	std::vector<ControllerDriver*> controllerDrivers_;
	std::vector<ControllerDriverContext> controllers_;
	KeyboardControllerConfig keyboardControllerConfig_;
	
	using DriverIter = decltype(controllerDrivers_)::const_iterator;
	
	IOHIDManagerRef hidManager_;

	void buildKeyTranslationTable();
	void initControllers();
	void processSystemEvents();
	
	void handleKeyTransition(Key, bool isDown);
	
public:
	DevicesContext();
	
	void setKeyboardControllerConfig(const KeyboardControllerConfig& kbConfig) {
		keyboardControllerConfig_ = kbConfig;
	}

	void frame();
	
	const Keyboard& keyboard() const { return keyboard_; }
	Controller* controllerAtIndex(size32 index);
	ControllerDriverContext& createController();
	
	DriverIter driversBegin() const { return controllerDrivers_.cbegin(); }
	DriverIter driversEnd() const { return controllerDrivers_.cend(); }
};


} // ns device
} // ns stardazed

#endif
