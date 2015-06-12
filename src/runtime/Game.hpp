// ------------------------------------------------------------------
// runtime::Game - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RUNTIME_GAME_H
#define SD_RUNTIME_GAME_H

#include "system/Config.hpp"
#include "system/Time.hpp"
#include "runtime/Client.hpp"
#include "physics/GlobalTime.hpp"
#include "scene/SceneController.hpp"

namespace stardazed {
namespace runtime {


enum class GameRunState {
	Idle,
	Running
};


class Game {
	Client& client_;
	physics::GlobalTime baseTime_, globalTime_;
	scene::SceneController* controller_ = nullptr;

	time::Duration physicsFixedStepTime_ = time::hertz(50);
	time::Duration maxFrameTime_   = time::hertz(4);
	time::Duration minFrameTime_   = time::hertz(120); // only relevant in non-vsync context
	time::Duration physicsLag_ = time::zero();
	time::Duration previousFrameStartTime_ = time::zero();
	
	GameRunState runState_ = GameRunState::Idle;
	
	void simulationStep();
	
public:
	Game(Client& client);
	
	void setSceneController(scene::SceneController&);
	
	void mainLoop();

	// suspend/resume is called by Application when the app
	// becomes or ceases to be the frontmost app in the system, resp.
	void suspend();
	void resume();
};


} // ns runtime
} // ns stardazed

#endif
