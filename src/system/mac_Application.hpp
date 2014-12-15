// ------------------------------------------------------------------
// mac_Application - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_SYSTEM_MAC_APPLICATION_H
#define SD_SYSTEM_MAC_APPLICATION_H

#include "system/Config.hpp"
#include "util/ConceptTraits.hpp"

namespace stardazed {


class Application {
	Application() = delete;

	static bool quit_;
	static bool active_;

public:
	static void init();

	// -- active refers to app being frontmost
	static void setActive(bool);
	static bool isActive() { return active_; }
	
	// -- handling of Quit signal from system or user
	static void quitNow();
	static bool shouldQuit() { return quit_; }
	static void resetShouldQuitFlag();
};


} // ns stardazed

#endif
