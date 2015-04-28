// ------------------------------------------------------------------
// scene::Renderable - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_SCENE_RENDERABLE_H
#define SD_SCENE_RENDERABLE_H

#include "system/Config.hpp"
#include "math/Matrix.hpp"
#include "scene/Scene.hpp"

namespace stardazed {
namespace scene {
	

struct RenderPassInfo {
	Scene& scene;
	Camera& camera;
	math::Mat4 projMat, viewMat, viewProjMat;
	
	RenderPassInfo(Scene& scene, Camera& cam)
	: scene (scene)
	, camera (cam)
	, projMat { cam.projectionMatrix() }
	, viewMat { cam.viewMatrix() }
	, viewProjMat { projMat * viewMat }
	{}
};


class Renderable {
public:
	virtual ~Renderable() = default;
	
	virtual void render(const RenderPassInfo&, const Entity& entity) const = 0;
};


} // ns scene
} // ns stardazed

#endif
