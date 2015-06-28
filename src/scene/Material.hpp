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


struct Material;


struct Renderable {
	bool castShadows = true;
	bool receiveShadows = true;

	Material* material = nullptr;
};


struct Material {
	virtual ~Material() = default;
	
	virtual void setup(render::RenderPass&, const ProjectionSetup&) = 0;
	virtual void render(const Renderable&) = 0;
	virtual void teardown() {}
};


enum class MaterialMode : uint {
	Opaque,
	Transparent
};


struct StandardMaterial {
	MaterialMode mode = MaterialMode::Opaque;
	math::Vec3 tintColor = math::Vec3::one();

	render::Texture* diffuseMap = nullptr;
	render::Texture* normalMap = nullptr;
};


/*
 3 parts:
 - Shader (Pipeline with uniform buffer/value knowledge, get/set)
	- has a render fn that takes instances of a Material
 - Material
	- a struct of Texture, vector and scalar info for rendering (uniforms)
 - Renderable
	- Component, render information (shadows, etc) + Material information
*/


} // ns scene
} // ns stardazed

#endif
