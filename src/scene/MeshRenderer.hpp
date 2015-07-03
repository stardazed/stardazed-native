// ------------------------------------------------------------------
// scene::MeshRenderer - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_SCENE_MATERIAL_H
#define SD_SCENE_MATERIAL_H

#include "system/Config.hpp"
#include "container/FlagSet.hpp"
#include "math/Matrix.hpp"
#include "render/common/RenderPass.hpp"

namespace stardazed {
namespace scene {


struct ProjectionSetup {
	math::Mat4 projMat, viewMat;
	
	ProjectionSetup(const math::Mat4& projectionMatrix, const math::Mat4& viewMatrix)
	: projMat { projectionMatrix }
	, viewMat { viewMatrix }
	{}
};


struct Entity;


struct Renderable {
	virtual ~Renderable() = default;
	virtual void render(render::RenderPass&, const ProjectionSetup&, const Entity&) const = 0;
};


struct MeshRenderer {
	bool castShadows = true;
	bool receiveShadows = true;
	Renderable* renderable = nullptr;
};


} // ns scene
} // ns stardazed

#endif
