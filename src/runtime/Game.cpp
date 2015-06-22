// ------------------------------------------------------------------
// runtime::Game - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#include "runtime/Game.hpp"
#include "system/Application.hpp"
#include "system/Logging.hpp"

#include <thread>

namespace stardazed {
namespace runtime {


struct FrameStatistics {
	Time startTime, endTime;
	Time inputEndTime {}, physicsEndTime {}, renderEndTime {}, swapEndTime {};
	Time lagDuration {};
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

	Time inputTime() { return inputEndTime - startTime; }
	Time physicsTime() { return physicsEndTime - inputEndTime; }
	Time renderTime() { return renderEndTime - physicsEndTime; }
	Time swapTime() { return swapEndTime - renderEndTime; }
	Time totalFrameTime() { return endTime - startTime; }
	
	void print() {
		auto input = time::asMicroseconds(inputTime());
		auto physics = time::asMicroseconds(physicsTime());
		auto render = time::asMicroseconds(renderTime());
		auto swap = time::asMicroseconds(swapTime());
		auto total = time::asMicroseconds(totalFrameTime());
		sd::log("input: ", input, "us phys: ", physics, "us rend: ", render, "us swap: ", swap, "us total: ", total, "us");
	}
};


Game::Game(Client& client)
: client_(client)
{}


void Game::setSceneController(scene::SceneController& ctl) {
	controller_ = &ctl;
}


void Game::simulationStep() {
	runtime::FrameContext frame{ client_, physicsFixedStepTime_ };
	auto& scene = controller_->scene();
	
	std::for_each(scene.entitiesBegin(), scene.entitiesEnd(),
		[&](scene::Entity& entity) {
			if (entity.behaviour)
				entity.behaviour->update(entity, scene, frame);
		});

	controller_->scene().physics().integrateStep(globalTime_, physicsFixedStepTime_);
}


void Game::mainLoop() {
	assert(controller_);

	if (runState_ != GameRunState::Idle)
		return;
	runState_ = GameRunState::Running;

	// -- set time base
	previousFrameStartTime_ = time::now();
	baseTime_ = previousFrameStartTime_;
	globalTime_ = baseTime_;

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
		globalTime_ += timeSinceLastFrameStart;

		client_.devices().frame();
		stats.inputDone();
		
		if (Application::isActive()) {
			physicsLag_ += timeSinceLastFrameStart;
			
			while (physicsLag_ >= physicsFixedStepTime_) {
				// run as many simulation frames as needed to catch up
				physicsLag_ -= physicsFixedStepTime_;
				simulationStep();
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

				if (sleepDuration > 0) {
					std::this_thread::sleep_for(std::chrono::duration<double>(sleepDuration));

//					auto afterSleepTime = time::now();
//					auto realSleepDuration = afterSleepTime - stats.endTime;
				}
			}
		}
		else {
			// game is not the active process, be nice
			std::this_thread::sleep_for(std::chrono::duration<double>(maxFrameTime_));
		}
		
		if (client_.devices().keyboard().isPressed(device::Key::F))
			stats.print();
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
