// ------------------------------------------------------------------
// scene::Renderable - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_SCENE_RENDERABLE_H
#define SD_SCENE_RENDERABLE_H

#include "system/Config.hpp"
#include "math/Matrix.hpp"
#include "render/common/RenderPass.hpp"
#include "scene/Scene.hpp"

namespace stardazed {
namespace scene {
	

struct RenderPassInfo {
	Scene& scene;
	math::Mat4 projMat, viewMat, viewProjMat;
	
	RenderPassInfo(Scene& scene, const math::Mat4& projectionMatrix, const math::Mat4& viewMatrix)
	: scene (scene)
	, projMat { projectionMatrix }
	, viewMat { viewMatrix }
	, viewProjMat { projMat * viewMat }
	{}
};


class Renderable {
public:
	virtual ~Renderable() = default;

	virtual void linkWithPipeline(render::Pipeline&) = 0;
	virtual void render(render::RenderPass&, const RenderPassInfo&, const Entity& entity) const = 0;
};


} // ns scene
} // ns stardazed

#endif
