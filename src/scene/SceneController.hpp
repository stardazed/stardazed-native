// ------------------------------------------------------------------
// SceneController - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_SCENE_SCENECONTROLLER_H
#define SD_SCENE_SCENECONTROLLER_H

#include "system/Config.hpp"
#include "scene/Scene.hpp"
#include "system/Time.hpp"

namespace stardazed {
namespace scene {


class SceneController {
	scene::Scene scene_;
	runtime::Client& client_;

public:
	SceneController(runtime::Client&);
	virtual ~SceneController() {}

	virtual void renderFrame(time::Duration) = 0;
	virtual void simulationFrame(time::Duration) = 0;

	Scene& scene() { return scene_; }
	runtime::Client& client() { return client_; }
};


} // ns scene
} // ns stardazed

#endif
