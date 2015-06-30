// ------------------------------------------------------------------
// scene::Material - stardazed
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


struct Material;


struct Renderable {
	bool castShadows = true;
	bool receiveShadows = true;

	Material* material = nullptr;
};


struct MaterialShader {
	virtual ~MaterialShader() = default;
	
	virtual void render(render::RenderPass& renderPass, const ProjectionSetup&, const Renderable&) const = 0;
};


enum class MaterialMode : uint {
	Opaque,
	Transparent
};


struct StandardMaterial {
	MaterialMode mode = MaterialMode::Opaque;
	float specularExponent = 1;

	math::Vec3 diffuseColour = math::Vec3::one();
	math::Vec3 specularColour = math::Vec3::one();

	render::Texture* diffuseMap = nullptr;
	render::Texture* normalMap = nullptr;
};


enum class StandardMaterialFlagsEnum : uint8 {
	UseDiffuseMap,
	UseNormalMap
};


using StandardMaterialFlags = container::FlagSet8<StandardMaterialFlagsEnum>;


struct CompiledStandardMaterial {
	GLint diffuseMap = 0, normalMap = 0;
	
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
