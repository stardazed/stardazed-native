// ------------------------------------------------------------------
// SceneController - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_SCENE_SCENECONTROLLER_HPP
#define SD_SCENE_SCENECONTROLLER_HPP

#include "system/Config.hpp"
#include "scene/Scene.hpp"
#include "scene/Renderable.hpp"
#include "runtime/Client.hpp"
#include "render/common/RenderPass.hpp"
#include "system/Time.hpp"

namespace stardazed {
namespace scene {


template <typename It>
Entity& entityFromIter(It it) { return *it; }

template <>
Entity& entityFromIter(container::RefTree<Entity>::Iterator it);


	
class SceneController {
	scene::Scene scene_;
	runtime::Client& client_;

	virtual void willResume() {}
	virtual void willSuspend() {}

public:
	SceneController(runtime::Client&);
	virtual ~SceneController() {}

	template <typename It>
	void renderEntityRange(render::RenderPass& renderPass, const RenderPassInfo& rpi, It from, It to) {
		for (It cur = from; cur != to; ++cur) {
			Entity& entity = entityFromIter(cur);
			
			// -- render entity
			if (entity.renderable) {
				entity.renderable->render(renderPass, rpi, entity);
			}
			
			// <-- render children
		}
	}

	virtual void renderFrame(time::Duration) = 0;
	void simulationFrame(time::Duration);

	Scene& scene() { return scene_; }
};


} // ns scene
} // ns stardazed

#endif
