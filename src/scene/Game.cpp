// ------------------------------------------------------------------
// sim::Game.cpp - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "system/Time.hpp"
#include "scene/Scene.hpp"
#include "scene/Camera.hpp"

namespace stardazed {


class Game {
public:
	Game();
	
	void mainLoop();
	void renderFrame(time::Duration x);
};


Game::Game() {
}


void renderCamera(const scene::Scene& scene, const scene::Camera& cam) {
	auto projMat = cam.projectionMatrix(),
		viewMat = cam.viewMatrix(),
		viewProjMat = projMat * viewMat;

//	auto root = scene.root();
	
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
			renderFrame(simulationLag);
			
			// render single frame, drop any missed ones
			while (renderLag >= renderFrameTime)
				renderLag -= renderFrameTime;
		}
	}
}
	
	
} // ns stardazed
