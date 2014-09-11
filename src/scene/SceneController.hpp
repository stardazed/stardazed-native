// ------------------------------------------------------------------
// SceneController - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_SCENE_SCENECONTROLLER_HPP
#define SD_SCENE_SCENECONTROLLER_HPP

#include "scene/Scene.hpp"
#include "runtime/Client.hpp"
#include "system/Time.hpp"

namespace stardazed {
namespace scene {
	
	
class SceneController {
	scene::Scene scene_;
	runtime::Client& client_;

	virtual void willResume() {}
	virtual void willSuspend() {}

public:
	SceneController(runtime::Client&);
	virtual ~SceneController() {}

	void simulationFrame(time::Duration);
	void renderFrame(time::Duration);

	Scene& scene() { return scene_; }
};


} // ns scene
} // ns stardazed

#endif
