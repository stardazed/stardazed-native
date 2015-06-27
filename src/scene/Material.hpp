// ------------------------------------------------------------------
// scene::Material - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_SCENE_MATERIAL_H
#define SD_SCENE_MATERIAL_H

#include "system/Config.hpp"
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


struct Renderable;


struct Material {
	virtual ~Material() = default;
	
	virtual void setup(render::RenderPass&, const ProjectionSetup&) = 0;
	virtual void render(const Renderable&) = 0;
	virtual void teardown() {}
};


struct Renderable {
	bool castShadows = true;
	bool receiveShadows = true;

	Material* material = nullptr;
};


} // ns scene
} // ns stardazed

#endif
