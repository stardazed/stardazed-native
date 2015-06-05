// ------------------------------------------------------------------
// SceneController - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "scene/SceneController.hpp"
#include "scene/Camera.hpp"
#include "scene/Entity.hpp"
#include "scene/Transform.hpp"
#include "runtime/FrameContext.hpp"

#include <algorithm>

namespace stardazed {
namespace scene {

	
template <>
Entity& entityFromIter(container::RefTree<Entity>::Iterator it) { return (*it)->item(); }


SceneController::SceneController(runtime::Client& client)
: client_(client)
{}


void SceneController::simulationFrame(time::Duration deltaTime) {
	// very basic for now
	runtime::FrameContext frame{ client_, deltaTime };

	std::for_each(scene_.allEntitiesBegin(), scene_.allEntitiesEnd(),
		[this,&frame](scene::Entity& entity){
			if (entity.behaviour)
				entity.behaviour->update(entity, scene_, frame);
		});
}


} // ns scene
} // ns stardazed
