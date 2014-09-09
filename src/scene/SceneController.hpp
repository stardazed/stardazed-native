// ------------------------------------------------------------------
// SceneController - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_SCENE_SCENECONTROLLER_HPP
#define SD_SCENE_SCENECONTROLLER_HPP

#include "system/Time.hpp"
#include "scene/Scene.hpp"

namespace stardazed {
namespace scene {
	
	
class SceneController {
	scene::Scene scene_;

	virtual void willResume() {}
	virtual void willSuspend() {}

public:
	virtual ~SceneController() {}

	void simulationFrame();
	void renderFrame();

	Scene& scene() { return scene_; }
};


} // ns scene
} // ns stardazed

#endif
