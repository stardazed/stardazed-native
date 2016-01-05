// ------------------------------------------------------------------
// RunLoop - stardazed
// (c) 2016 by Arthur Langereis
// ------------------------------------------------------------------

#include "runtime/RunLoop.hpp"
#include "system/Application.hpp"
#include "system/Logging.hpp"
#include "io/input.hpp"

#include <thread>

namespace stardazed {


RunLoop::RunLoop()
{}


void RunLoop::setSceneController(SceneController& ctl) {
	controller_ = &ctl;
}


void RunLoop::setRenderContext(render::RenderContext& rc) {
	renderCtx_ = &rc;
}


void RunLoop::mainLoop() {
	assert(renderCtx_);
	
	if (runState_ != RunLoopState::Idle)
		return;
	runState_ = RunLoopState::Running;
	
	// -- set time base
	lastFrameTime_ = time::now();
	globalTime_ = 0;
	
	while (! Application::shouldQuit()) {
		auto frameStartTime = time::now();
		auto timeSinceLastFrameStart = frameStartTime - lastFrameTime_;
		lastFrameTime_ = frameStartTime;

		io::update();
		
		if (Application::isActive()) {
			if (timeSinceLastFrameStart > maxFrameTime_) {
				// if we exceed the max frame time then we will start introducing
				// real lag and slowing the game down to catch up
				timeSinceLastFrameStart = maxFrameTime_;
			}
			globalTime_ += timeSinceLastFrameStart;
			
			if (controller_) {
				controller_->simulationStep(timeSinceLastFrameStart);
				controller_->renderFrame(timeSinceLastFrameStart);
			}

			renderCtx_->swap();
			
			auto totalFrameTime = time::now() - frameStartTime;
			
			if (! renderCtx_->usesVerticalSync()) {
				auto sleepDuration = minFrameTime_ - totalFrameTime;
				
				if (sleepDuration > 0) {
					std::this_thread::sleep_for(std::chrono::duration<double>(sleepDuration));
				}
			}
		}
		else {
			// game is not the active process, be nice
			std::this_thread::sleep_for(std::chrono::duration<double>(maxFrameTime_));
		}
	}
	
	runState_ = RunLoopState::Idle;
}


void RunLoop::start() {
	if (runState_ != RunLoopState::Idle)
		return;
	
	lastFrameTime_ = time::now() - minFrameTime_;
	runState_ = RunLoopState::Running;
}


void RunLoop::stop() {
	if (runState_ != RunLoopState::Running)
		return;
	runState_ = RunLoopState::Idle; // FIXME: Stopping -> Idle with async notif of stopped
}


RunLoop& defaultRunLoop() {
	static RunLoop defaultRL{};
	return defaultRL;
}

	
} // ns stardazed
