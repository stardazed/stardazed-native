// ------------------------------------------------------------------
// runtime::RunLoop - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RUNTIME_RUNLOOP_H
#define SD_RUNTIME_RUNLOOP_H

#include "system/Application.hpp"
#include "runtime/Client.hpp"

namespace stardazed {
namespace runtime {
	

class RunLoop {
	Application& app_;
	Client& client_;
	bool running_;
	
	
	void suspend();
	void resume();
	void mainLoop();

public:
	RunLoop(Application& app, Client& client);
	
	void run();
};
	
	
} // ns runtime
} // ns stardazed

#endif
