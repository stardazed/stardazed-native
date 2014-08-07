// ------------------------------------------------------------------
// Game - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_GAME_HPP
#define SD_GAME_HPP

#include "system/Time.hpp"
#include "scene/Scene.hpp"

namespace stardazed {
	
	
class Game {
	time::Duration renderFrameTime_ = time::hertz(60);
	time::Duration simulationFrameTime_ = time::hertz(120);
	time::Duration simulationLag_ = time::zero(), renderLag_ = time::zero();
	time::Duration previousTime_;
	
	scene::Scene* scene_ = nullptr;
	
public:
	void renderFrame(time::Duration lag);
	void simulationFrame();
	void step();
	
	void setScene(scene::Scene& sc) { scene_ = &sc; }
	
//	void mainLoop();
};

	
} // ns stardazed

#endif
