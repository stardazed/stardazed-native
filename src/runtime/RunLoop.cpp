// ------------------------------------------------------------------
// runtime::RunLoop - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "runtime/RunLoop.hpp"

#include <cmath>
#include <thread>

namespace stardazed {
namespace runtime {


RunLoop::RunLoop(Application& app, Client& client)
: app_(app)
, client_(client)
, running_(false)
{}


void RunLoop::step() {
	auto currentTime = time::now();
	auto elapsedTime = currentTime - previousTime_;
	previousTime_ = currentTime;
	
	simulationLag_ += elapsedTime;
	renderLag_ += elapsedTime;
	
	// <-- process input / engine events
	
	while (simulationLag_ >= simulationFrameTime_) {
		// run as many simulation frames as needed to catch up
		simulationLag_ -= simulationFrameTime_;
		simulationFrame();
	}
	
	if (renderLag_ >= renderFrameTime_) {
		// render single frame, drop any missed ones
		renderLag_ -= renderFrameTime_ * std::floor(renderLag_ / renderFrameTime_);
		renderFrame(simulationLag_);
	}
}


void RunLoop::mainLoop() {
	while (! app_.shouldQuit()) {
		client.devices().frame();
		
		if (app_.isActive()) {
//			sceneRunner.simulationFrame();
//			sceneRunner.renderFrame(sd::time::zero());
			client.render().swap();
		}
	}
}


void RunLoop::suspend() {
	
}


void RunLoop::resume() {
	
}


void RunLoop::run() {
	resume();
	mainLoop();
}


} // ns runtime
} // ns stardazed
