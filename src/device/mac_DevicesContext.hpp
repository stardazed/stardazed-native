// ------------------------------------------------------------------
// device::mac_DevicesContext - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_DEVICE_MAC_DEVICESCONTEXT_H
#define SD_DEVICE_MAC_DEVICESCONTEXT_H

#include "system/Config.hpp"
#include "device/Keyboard.hpp"
#include "device/Controller.hpp"

#include <array>

#include <IOKit/hid/IOHIDLib.h>

namespace stardazed {
namespace device {


struct KeyboardControllerConfig {
	Key leftStickLeft, leftStickRight, leftStickUp, leftStickDown;
	Key rightStickLeft, rightStickRight, rightStickUp, rightStickDown;
	Key dPadLeft, dPadRight, dPadUp, dPadDown;

	Key btnA, btnB, btnX, btnY, btnL1, btnR1, btnSelect, btnStart;
	// L2 and R2 not supported by definition (analog triggers)
};


struct ControllerSet {
	std::array<Controller, 8> controllers;

	const Controller* begin() const { return std::begin(controllers); }
	const Controller* end() const   { return std::end(controllers); }
};


class DevicesContext {
	Keyboard keyboard_;
	std::array<Key, 512> keyTransTable_;

	ControllerSet controllers_;
	KeyboardControllerConfig keyboardControllerConfig_;
	
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
	const ControllerSet& controllers() const { return controllers_; }
};


} // ns device
} // ns stardazed

#endif
