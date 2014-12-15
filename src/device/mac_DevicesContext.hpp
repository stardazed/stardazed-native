// ------------------------------------------------------------------
// device::mac_DevicesContext - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_DEVICE_MAC_DEVICESCONTEXT_H
#define SD_DEVICE_MAC_DEVICESCONTEXT_H

#include "system/Config.hpp"
#include "device/Keyboard.hpp"

#include <array>

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


struct KeyboardControllerConfig {
	Key leftStickLeft, leftStickRight, leftStickUp, leftStickDown;
	Key rightStickLeft, rightStickRight, rightStickUp, rightStickDown;
	Key dPadLeft, dPadRight, dPadUp, dPadDown;

	Key btnA, btnB, btnX, btnY, btnL1, btnR1, btnSelect, btnStart;
	// L2 and R2 not supported by definition (analog triggers)
};


struct ControllerSet {
	static const int MAX_CONTROLLERS = 8;
	Controller controllers[MAX_CONTROLLERS];

	const Controller* begin() const { return std::begin(controllers); }
	const Controller* end() const   { return std::end(controllers); }
};


class DevicesContext {
	Keyboard keyboard_;
	std::array<Key, 512> keyTransTable_;
	ControllerSet controllers_;
	KeyboardControllerConfig keyboardControllerConfig;

	void buildKeyTranslationTable();
	void processSystemEvents();
	
	void handleKeyTransition(Key, bool isDown);
	
public:
	DevicesContext();
	
	void setKeyboardControllerConfig(const KeyboardControllerConfig& kbConfig) {
		keyboardControllerConfig = kbConfig;
	}

	void frame();
	
	const Keyboard& keyboard() const { return keyboard_; }
	const ControllerSet& controllers() const { return controllers_; }
};


} // ns device
} // ns stardazed

#endif
