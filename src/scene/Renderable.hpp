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
	

class Renderable {
public:
	virtual ~Renderable() = default;

	virtual void linkWithPipeline(render::Pipeline&) = 0;
	virtual void render(render::RenderPass&, const RenderPassInfo&, const Entity& entity) const = 0;
};


} // ns scene
} // ns stardazed

#endif
