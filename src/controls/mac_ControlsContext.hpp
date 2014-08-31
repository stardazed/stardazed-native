// ------------------------------------------------------------------
// mac_ControlsContext - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_CONTROLS_MAC_CONTROLSCONTEXT_H
#define SD_CONTROLS_MAC_CONTROLSCONTEXT_H

#include "event/EventService.hpp"

#include <bitset>

namespace stardazed {
namespace controls {

	
class ControlsContext {
	std::bitset<128> keyPressTable_;
	event::EventService& events_;

public:
	ControlsContext(event::EventService&);

	// -- Keyboard
	bool isKeyPressed(event::Key);
	
	// -- Mouse
	
	// -- Joysticks
	
	// -- Sensors
	
};
	
	
} // ns controls
} // ns stardazed

#endif
