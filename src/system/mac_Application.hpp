// ------------------------------------------------------------------
// mac_Application - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_SYSTEM_MAC_APPLICATION_H
#define SD_SYSTEM_MAC_APPLICATION_H

#include "util/ConceptTraits.hpp"

namespace stardazed {


class Application {
	SD_NOCOPYORMOVE_CLASS(Application)

	bool quit_ = false;
	bool active_ = false;
	
	void changeToResourcesDirectory();

public:
	Application();
	
	// -- active refers to app being frontmost
	void setActive(bool);
	bool isActive() const { return active_; }
	
	// -- handling of Quit signal from system or user
	void quitNow();
	bool shouldQuit() const { return quit_; }
	void resetShouldQuitFlag();
};


} // ns stardazed

#endif
