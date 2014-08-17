// ------------------------------------------------------------------
// input::InputContext - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_INPUT_INPUTCONTEXT_H
#define SD_INPUT_INPUTCONTEXT_H

#include "util/ConceptTraits.hpp"
#include "input/Keyboard.hpp"

namespace stardazed {
namespace input {


class InputContext {
	SD_NOCOPYORMOVE_CLASS(InputContext)

public:
	InputContext() {}
	virtual ~InputContext() = default;
	
	virtual void processSystemEvents() = 0;

	// -- Keyboard
	virtual bool isKeyPressed(Key) = 0;
	
	// -- Mouse
	
	// -- Joysticks
	
	// -- Sensors
};


} // ns input
} // ns stardazed

#endif
