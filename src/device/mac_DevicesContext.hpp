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


class DevicesSnapshot {
	KeyboardSnapshot keyboard_;
	
public:
	DevicesSnapshot(KeyboardSnapshot);
	const KeyboardSnapshot& keyboard() const;
};


class DevicesContext {
	Keyboard keyboard_;
	std::array<Key, 512> keyTransTable_;

	void buildKeyTranslationTable();
	void processSystemEvents();
	
	void handleKeyDown(Key);
	void handleKeyUp(Key);
	
public:
	DevicesContext();

	void frame();
	const DevicesSnapshot snapshot();

	// TODO: some things that would go in here
//	void discoverJoysticks();
//	
//	void lockPointer();
//	void unlockPointer();
//	void movePointerTo(Vec2);
};


} // ns device
} // ns stardazed

#endif
