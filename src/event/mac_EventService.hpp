// ------------------------------------------------------------------
// event::mac_EventService - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_EVENT_MAC_EVENTSERVICE_H
#define SD_EVENT_MAC_EVENTSERVICE_H

#include "event/Keyboard.hpp"
#include <array>

namespace stardazed {
namespace event {


class EventService {
	std::array<Key, 512> keyTransTable_;
	
	void buildKeyTranslationTable();
	void handleKeyDown(Key);
	void handleKeyUp(Key);

public:
	EventService();

	void processSystemEvents();
};


} // ns event
} // ns stardazed

#endif
