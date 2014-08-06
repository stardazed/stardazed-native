// ------------------------------------------------------------------
// sim::Game.cpp - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "system/Time.hpp"
#include "scene/Scene.hpp"
#include "scene/Camera.hpp"

namespace stardazed {


class Game {
	time::Duration simulationFrameTime_, renderFrameTime_;
	time::Duration simulationLag_, renderLag_;
	time::Duration previousTime_;
	
	void step();

public:
	Game();
	
	void mainLoop();
	void renderFrame(time::Duration x);
};


Game::Game() {
	simulationFrameTime_ = time::hertz(120);
	renderFrameTime_     = time::hertz(60);
	
	simulationLag_       = time::zero();
	renderLag_           = time::zero();
}


void renderCamera(const scene::Scene& scene, const scene::Camera& cam) {
	using namespace scene;

	auto projMat = cam.projectionMatrix(),
		viewMat = cam.viewMatrix(),
		viewProjMat = projMat * viewMat;

	
}


void Game::renderFrame(time::Duration) {
//	for (auto camera : Cameras) {
//		auto objs = scene.getObjectsWithMask(camera.objectMask);
//		objs.clipAgainstCameraFrustum(camera);
//		
//		for (auto& obj : objs) {
//			obj.render();
//		}
//	}
}


void Game::step() {
	auto currentTime = time::now();
	auto elapsedTime = currentTime - previousTime_;
	previousTime_ = currentTime;
	
	simulationLag_ += elapsedTime;
	renderLag_ += elapsedTime;

	// process input
	
	while (simulationLag_ >= simulationFrameTime_) {
		// update physics, AI, etc.

		simulationLag_ -= simulationFrameTime_;
	}
	
	if (renderLag_ >= renderFrameTime_) {
		// render frame
		renderFrame(simulationLag_);
		
		// render single frame, drop any missed ones
		while (renderLag_ >= renderFrameTime_)
			renderLag_ -= renderFrameTime_;
	}
}
	
	
} // ns stardazed
