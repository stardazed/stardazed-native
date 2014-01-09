// ------------------------------------------------------------------
// Application - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include <memory>

#ifndef SD_APPLICATION_H
#define SD_APPLICATION_H

namespace stardazed {

	class Application {
	public:
		Application();
		
		void yieldSystem();
		bool shouldQuit();
	};

	void quitNow();

}

#endif
