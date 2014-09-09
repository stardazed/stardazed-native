// ------------------------------------------------------------------
// Game.cpp - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "scene/Game.hpp"
#include "scene/Camera.hpp"
#include "scene/Entity.hpp"
#include "scene/Transform.hpp"

#include <algorithm>

namespace stardazed {


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


void Game::renderFrame(time::Duration) {
	auto& cam = *(scene_->camerasBegin()); // boom
	RenderPass rp { *scene_, cam };
	rp.render();
}


void Game::simulationFrame() {
	// very basic for now
	std::for_each(scene_->allEntitiesBegin(), scene_->allEntitiesEnd(), [](scene::Entity& entity){
		if (entity.behaviour)
			entity.behaviour->update(entity);
	});
}


void Game::step() {
	auto currentTime = time::now();
	auto elapsedTime = currentTime - previousTime_;
	previousTime_ = currentTime;
	
	simulationLag_ += elapsedTime;
	renderLag_ += elapsedTime;

	// <-- process input / engine events
	
	while (simulationLag_ >= simulationFrameTime_) {
		// run as many simulation frames as needed to catch up
		simulationLag_ -= simulationFrameTime_;
		simulationFrame();
	}
	
	if (renderLag_ >= renderFrameTime_) {
		// render single frame, drop any missed ones
		renderLag_ -= renderFrameTime_ * std::floor(renderLag_ / renderFrameTime_);
		renderFrame(simulationLag_);
	}
}


} // ns stardazed
