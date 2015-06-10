// ------------------------------------------------------------------
// mac_Application - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_SYSTEM_MAC_APPLICATION_H
#define SD_SYSTEM_MAC_APPLICATION_H

#include "system/Config.hpp"

namespace stardazed {


namespace runtime {
	class Game;
}


class Application {
	Application() = delete;

	static bool quit_;
	static bool active_;
	static runtime::Game* game_;

public:
	static void init();
	
	static void setGame(runtime::Game& game) {
		game_ = &game;
	}

	// -- active refers to app being frontmost
	static void setActive(bool active);
	static bool isActive() { return active_; }
	
	// -- handling of Quit signal from system or user
	static bool shouldQuit() { return quit_; }
	static void quitNow() { quit_ = true; }
	static void resetShouldQuitFlag() { quit_ = false; }
};


} // ns stardazed

#endif
