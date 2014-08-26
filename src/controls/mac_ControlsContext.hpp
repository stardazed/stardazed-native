// ------------------------------------------------------------------
// mac_ControlsContext - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_CONTROLS_CONTROLSCONTEXT_H
#define SD_CONTROLS_CONTROLSCONTEXT_H

#include "event/Keyboard.hpp"

#include <array>
#include <bitset>

namespace stardazed {
namespace controls {

	
class ControlsContext {
	std::array<event::Key, 512> keyTransTable_;
	std::bitset<128> keyPressTable_;

	void buildKeyTranslationTable();
	void handleKeyDown(event::Key);
	void handleKeyUp(event::Key);

public:
	ControlsContext();
	
	void processSystemEvents();

	// -- Keyboard
	bool isKeyPressed(event::Key);
	
	// -- Mouse
	
	// -- Joysticks
	
	// -- Sensors
	
};
	
	
} // ns controls
} // ns stardazed

#endif
