// ------------------------------------------------------------------
// Application - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_SYSTEM_APPLICATION_H
#define SD_SYSTEM_APPLICATION_H

#include "util/ConceptTraits.hpp"
#include "input/InputContext.hpp"

namespace stardazed {


class Application {
	SD_NOCOPYORMOVE_CLASS(Application)

public:
	Application() {}
	virtual ~Application() = default;
	
	// -- InputContext access
	virtual input::InputContext& input() = 0;

	// -- handling of Quit signal from system or user
	virtual void quitNow() = 0;
	virtual bool shouldQuit() = 0;
	virtual void resetShouldQuitFlag() = 0;
};

	
} // ns stardazed

#endif
