// ------------------------------------------------------------------
// SceneController - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#include "scene/SceneController.hpp"
#include "scene/Camera.hpp"
#include "scene/Entity.hpp"
#include "runtime/FrameContext.hpp"

#include <algorithm>

namespace stardazed {
namespace scene {

	
SceneController::SceneController(runtime::Client& client)
: client_(client)
{}


void SceneController::simulationFrame(time::Duration deltaTime) {
	// very basic for now
	runtime::FrameContext frame{ client_, deltaTime };

	std::for_each(scene_.entitiesBegin(), scene_.entitiesEnd(),
		[this,&frame](scene::Entity& entity){
			if (entity.behaviour)
				entity.behaviour->update(entity, scene_, frame);
		});
}


} // ns scene
} // ns stardazed
