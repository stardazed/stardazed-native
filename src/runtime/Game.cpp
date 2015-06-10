// ------------------------------------------------------------------
// runtime::Game - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#include "runtime/Game.hpp"
#include "system/Application.hpp"
#include "system/Logging.hpp"

#include <cmath>
#include <thread>

namespace stardazed {
namespace runtime {


struct FrameStatistics {
	time::Duration startTime, endTime;
	time::Duration inputEndTime {}, physicsEndTime {}, renderEndTime {}, swapEndTime {};
	time::Duration lagDuration {};
	int physicsStepsRun = 0;
	
	FrameStatistics()
	: startTime(time::now())
	{}
	
	// --
	void inputDone() { inputEndTime = time::now();	}
	
	void physicsStep() { ++physicsStepsRun; }
	void physicsDone() { physicsEndTime = time::now(); }
	
	void renderDone() { renderEndTime = time::now(); }
	void swapDone() { swapEndTime = time::now(); }
	
	void frameDone() { endTime = time::now(); }
	
	// --
	time::Duration inputTime() { return inputEndTime - startTime; }
	time::Duration physicsTime() { return physicsEndTime - inputEndTime; }
	time::Duration renderTime() { return renderEndTime - physicsEndTime; }
	time::Duration swapTime() { return swapEndTime - renderEndTime; }
	time::Duration totalFrameTime() { return endTime - startTime; }
	
	void print() {
		using namespace std::chrono;

		auto input = duration_cast<microseconds>(inputTime()).count();
		auto physics = duration_cast<microseconds>(physicsTime()).count();
		auto render = duration_cast<microseconds>(renderTime()).count();
		auto swap = duration_cast<microseconds>(swapTime()).count();
		auto total = duration_cast<microseconds>(totalFrameTime()).count();
		sd::log("input: ", input, "us phys: ", physics, "us rend: ", render, "us swap: ", swap, "us total: ", total, "us");
	}
};


Game::Game(Client& client)
: client_(client)
{}


void Game::setSceneController(scene::SceneController& ctl) {
	controller_ = &ctl;
}


void Game::mainLoop() {
	if (runState_ != GameRunState::Idle)
		return;

	previousFrameStartTime_ = time::now();
	runState_ = GameRunState::Running;

	while (! Application::shouldQuit()) {
		FrameStatistics stats;
	
		auto timeSinceLastFrameStart = stats.startTime - previousFrameStartTime_;
		previousFrameStartTime_ = stats.startTime;
		
		if (timeSinceLastFrameStart > maxFrameTime_) {
			// if we exceed the max frame time then we will start introducing
			// real lag and slowing the game down to catch up
			stats.lagDuration = timeSinceLastFrameStart - maxFrameTime_;
			timeSinceLastFrameStart = maxFrameTime_;
		}
		globalTimePoint_ += timeSinceLastFrameStart;

		client_.devices().frame();
		stats.inputDone();
		
		if (Application::isActive()) {
			physicsLag_ += timeSinceLastFrameStart;
			
			while (physicsLag_ >= physicsFixedStepTime_) {
				// run as many simulation frames as needed to catch up
				physicsLag_ -= physicsFixedStepTime_;
				controller_->simulationFrame(physicsFixedStepTime_);
				stats.physicsStep();
			}
			stats.physicsDone();
			
			controller_->renderFrame(timeSinceLastFrameStart);
			stats.renderDone();
			
			client_.render().swap();
			stats.swapDone();

			stats.frameDone();
			
			if (! client_.render().usesVerticalSync()) {
				auto sleepDuration = minFrameTime_ - stats.totalFrameTime();

				if (sleepDuration.count() > 0) {
					std::this_thread::sleep_for(sleepDuration);

//					auto afterSleepTime = time::now();
//					auto realSleepDuration = afterSleepTime - stats.endTime;
				}
			}
		}
		else {
			// game is not the active process, be nice
			std::this_thread::sleep_for(maxFrameTime_);
		}
		
//		stats.print();
	}
	
	runState_ = GameRunState::Idle;
}


void Game::resume() {
	if (runState_ != GameRunState::Idle)
		return;

	previousFrameStartTime_ = time::now();
	runState_ = GameRunState::Running;
}


void Game::suspend() {
	if (runState_ != GameRunState::Running)
		return;
	runState_ = GameRunState::Idle; // FIXME: Stopping -> Idle with async notif of stopped
}


} // ns runtime
} // ns stardazed
