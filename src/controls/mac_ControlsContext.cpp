// ------------------------------------------------------------------
// mac_ControlsContext - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "system/Logging.hpp"
#include "controls/mac_ControlsContext.hpp"

namespace stardazed {
namespace controls {


ControlsContext::ControlsContext(event::EventService& events)
: events_(events)
{
}


bool ControlsContext::isKeyPressed(event::Key key) {
	return keyPressTable_.test(static_cast<size_t>(key));
}

	
} // ns controls
} // ns stardazed
