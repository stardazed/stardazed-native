// ------------------------------------------------------------------
// sim::Game.cpp - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "system/Time.hpp"
#include "common/RingBuffer.hpp"

namespace stardazed {


class Game {
public:
	Game();
	
	void mainLoop();
};


Game::Game() {
}


void Game::mainLoop() {
	auto previousTime = time::now();
	auto simulationLag = time::zero(), renderLag = time::zero();
	
	const auto simulationFrameTime = time::hertz(120);
	const auto renderFrameTime = time::hertz(60);
	
	while (1) {
		auto currentTime = time::now();
		auto elapsedTime = currentTime - previousTime;
		previousTime = currentTime;
		
		simulationLag += elapsedTime;
		renderLag += elapsedTime;

		// process input
		
		while (simulationLag >= simulationFrameTime) {
			// update physics, AI, etc.

			simulationLag -= simulationFrameTime;
		}
		
		if (renderLag >= renderFrameTime) {
			// render frame
			
			// render single frame, drop any missed ones
			while (renderLag >= renderFrameTime)
				renderLag -= renderFrameTime;
		}
	}
}
	
	
} // ns stardazed
