// ------------------------------------------------------------------
// MacInputContext - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_INPUT_MACINPUTCONTEXT_H
#define SD_INPUT_MACINPUTCONTEXT_H

#include "input/InputContext.hpp"

#include <array>

namespace stardazed {
namespace input {

	
class MacInputContext : public InputContext {
	std::array<Key, 512> keyTransTable_;
	void buildKeyTranslationTable();

public:
	MacInputContext();
	
	void processSystemEvents() override;

	// -- Keyboard
	bool isKeyPressed(Key) override;
	
	// -- Mouse
	
	// -- Joysticks
	
	// -- Sensors
	
};
	
	
} // ns input
} // ns stardazed

#endif
