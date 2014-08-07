// ------------------------------------------------------------------
// Game.cpp - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "scene/Game.hpp"
#include "scene/Camera.hpp"
#include "scene/Node.hpp"
#include "scene/Transform.hpp"

#include <algorithm>

namespace stardazed {


template <typename It>
inline scene::Node& nodeFromIter(It it) { return *it; }

template <>
inline scene::Node& nodeFromIter(container::RefTree<scene::Node>::Iterator it) { return (*it)->item(); }


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
	void renderNodeRange(It from, It to) {
		for (It cur = from; cur != to; ++cur) {
			scene::Node& node = nodeFromIter(cur);
			
			if (node.mesh && node.pipeline) {
				auto modelViewMat = viewMat_ * node.transform.toMatrix4();

				auto constantBuffer = node.pipeline->constantBuffer();
				constantBuffer->setModelViewMatrix(modelViewMat);
				constantBuffer->setModelViewProjectionMatrix(projMat_ * modelViewMat);
				constantBuffer->setNormalMatrix(math::extractSubMatrix<3, 3>(modelViewMat));
				
				node.pipeline->activate();
				node.mesh->draw();
			}
			
			// <-- render children
		}
	}


	void render() {
		auto rootBegin = scene_.rootNodesBegin();
		auto rootEnd   = scene_.rootNodesEnd();
		
		renderNodeRange(rootBegin, rootEnd);
	}
};


void Game::renderFrame(time::Duration) {
	auto cam = *(scene_->camerasBegin()); // boom
	RenderPass rp { *scene_, cam };
	rp.render();
}


void Game::simulationFrame() {
	// very basic for now
	std::for_each(scene_->allNodesBegin(), scene_->allNodesEnd(), [](scene::Node& node){
		if (node.behaviour)
			node.behaviour->update(node);
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
