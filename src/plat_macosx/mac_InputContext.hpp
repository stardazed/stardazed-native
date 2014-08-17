// ------------------------------------------------------------------
// MacInputContext - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_INPUT_MACINPUTCONTEXT_H
#define SD_INPUT_MACINPUTCONTEXT_H

#include "input/InputContext.hpp"

#include <array>
#include <bitset>

namespace stardazed {
namespace input {

	
class MacInputContext : public InputContext {
	std::array<Key, 512> keyTransTable_;
	std::bitset<128> keyPressTable_;

	void buildKeyTranslationTable();
	void handleKeyDown(Key);
	void handleKeyUp(Key);

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
