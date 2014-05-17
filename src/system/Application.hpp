// ------------------------------------------------------------------
// Application - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_SYSTEM_APPLICATION_H
#define SD_SYSTEM_APPLICATION_H

#include "util/ConceptTraits.hpp"

namespace stardazed {


class Application : public NoCopyAllowedTrait {
public:
	Application();
	
	void yieldSystem();
	bool shouldQuit();
};

void quitNow();

	
} // ns stardazed

#endif
