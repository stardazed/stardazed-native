// ------------------------------------------------------------------
// SceneController - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "scene/SceneController.hpp"
#include "scene/Camera.hpp"
#include "scene/Entity.hpp"
#include "scene/Transform.hpp"

#include <algorithm>

#include <OpenGL/gl3.h>

namespace stardazed {
namespace scene {


template <typename It>
inline scene::Entity& entityFromIter(It it) { return *it; }

template <>
inline scene::Entity& entityFromIter(container::RefTree<scene::Entity>::Iterator it) { return (*it)->item(); }


class RenderPass {
	math::Mat4 projMat_, viewMat_, viewProjMat_;
	scene::Scene& scene_;
	
public:
	RenderPass(scene::Scene& scene, const scene::Camera& cam)
	: projMat_ { cam.projectionMatrix() }
	, viewMat_ { cam.viewMatrix() }
	, viewProjMat_ { projMat_ * viewMat_ }
	, scene_ { scene }
	{}

	template <typename It>
	void renderEntityRange(It from, It to) {
		for (It cur = from; cur != to; ++cur) {
			scene::Entity& entity = entityFromIter(cur);

			if (entity.mesh && entity.pipeline) {
				auto modelViewMat = viewMat_ * entity.transform.toMatrix4();

				auto constantBuffer = entity.pipeline->constantBuffer();
				constantBuffer->setModelViewMatrix(modelViewMat);
				constantBuffer->setModelViewProjectionMatrix(projMat_ * modelViewMat);

				// inverse + transpose only necessary if a non-uniform scale factor is applied
				auto normalMat = math::transpose(math::inverse(math::extractSubMatrix<3, 3>(modelViewMat)));
				constantBuffer->setNormalMatrix(normalMat);
				
				entity.pipeline->activate();
				entity.mesh->draw();
			}

			// <-- render children
		}
	}

	void render() {
		auto rootBegin = scene_.rootEntitiesBegin();
		auto rootEnd   = scene_.rootEntitiesEnd();
		
		renderEntityRange(rootBegin, rootEnd);
	}
};


SceneController::SceneController(runtime::Client& client)
: client_(client)
{}


void SceneController::renderFrame(time::Duration) {
	// *cough*
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	auto& cam = *(scene_.camerasBegin()); // boom
	RenderPass rp { scene_, cam };
	rp.render();
}


void SceneController::simulationFrame(time::Duration) {
	// very basic for now
	std::for_each(scene_.allEntitiesBegin(), scene_.allEntitiesEnd(), [this](scene::Entity& entity){
		if (entity.behaviour)
			entity.behaviour->update(entity, scene_, client_);
	});
}


} // ns scene
} // ns stardazed
