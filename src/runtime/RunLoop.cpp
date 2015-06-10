// ------------------------------------------------------------------
// runtime::RunLoop - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "runtime/RunLoop.hpp"
#include "system/Logging.hpp"

#include <cmath>
#include <thread>

namespace stardazed {
namespace runtime {


RunLoop::RunLoop(Client& client)
: client_(client)
, sceneCtl_(nullptr)
{
	// reasonable defaults
	setMaxRenderFPS(60);
	setSimulationFPS(60);
}


void RunLoop::setSceneController(scene::SceneController& ctl) {
	sceneCtl_ = &ctl;
}


void RunLoop::mainLoop() {
	previousTime_ = time::now();

	while (! Application::shouldQuit()) {
		auto currentTime = time::now();
		auto elapsedTime = currentTime - previousTime_;
		previousTime_ = currentTime;

		client_.devices().frame();
		
		if (Application::isActive()) {
			simulationLag_ += elapsedTime;
			renderLag_ += elapsedTime;
			
			while (simulationLag_ >= simulationFrameTime_) {
				// run as many simulation frames as needed to catch up
				simulationLag_ -= simulationFrameTime_;
				sceneCtl_->simulationFrame(simulationFrameTime_);
			}
			
			if (renderLag_ >= renderFrameTime_) {
				// render single frame, drop any missed ones
				renderLag_ -= renderFrameTime_ * std::floor(renderLag_ / renderFrameTime_);
				sceneCtl_->renderFrame(renderFrameTime_);
				client_.render().swap();
				stats.frameRendered();
			}
		}
		else {
			// game is not the active process, be nice
			// FIXME: has to be linked to some app event or something
			std::this_thread::sleep_for(time::milliseconds(500));
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
	sd::log(stats.fps(), "fps");
}


} // ns runtime
} // ns stardazed
