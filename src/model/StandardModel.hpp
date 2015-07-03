// ------------------------------------------------------------------
// model::StandardModel
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_MODEL_STANDARDMODEL_H
#define SD_MODEL_STANDARDMODEL_H

#include "system/Config.hpp"
#include "math/Vector.hpp"
#include "render/common/Mesh.hpp"
#include "render/common/Texture.hpp"
#include "render/common/Buffer.hpp"
#include "render/common/Pipeline.hpp"
#include "render/common/RenderPass.hpp"
#include "scene/MeshRenderer.hpp"
#include "scene/Entity.hpp"

#include <vector>

namespace stardazed {
namespace model {


struct StandardMaterial {
	// colours
	math::Vec3 mainColour = math::Vec3::one();        // single colour or tint for albedo
	float pad__ = 0;
	math::Vec3 specularColour = math::Vec3::zero();
	float specularExponent = 0;                       // 0 means no specular

	// textures
	math::Vec2 textureScale = math::Vec2::one();      // scale and offset apply to all textures
	math::Vec2 textureOffset = math::Vec2::zero();

	render::Texture* albedoMap = nullptr;             // nullptr means use mainColour only
	render::Texture* normalMap = nullptr;             // nullptr means no bump
};


struct FaceGroup {
	uint32 materialIx, fromFaceIx, toFaceIx;          // materialIx is index into materials array
};


struct StandardModelDescriptor {
	render::Mesh* mesh;
	std::vector<StandardMaterial> materials;
	std::vector<FaceGroup> faceGroups;
};


class StandardShader;


class StandardModel : public scene::Renderable {
	StandardModelDescriptor descriptor_;
	render::Mesh& mesh_;
	render::Buffer constMatBuffer_;
	StandardShader& shader_;

public:
	StandardModel(const StandardModelDescriptor&, StandardShader&);
	void render(render::RenderPass&, const scene::ProjectionSetup&, const scene::Entity&) const final;
};


class StandardShader {
	render::Pipeline* pipeline_;
	GLint vsMV, vsMVP, vsNM, fsNM;
	GLint fsLightDir, fsMatIndex;
//	GLint fsAlbedoMap, fsNormalMap;

public:
	StandardShader(render::RenderContext&);
	const render::Pipeline& pipeline() { return *pipeline_; }
	
	void applyMaterial(const StandardMaterial&);
	
	void setMatrices(const math::Mat4& projection, const math::Mat4& view, const math::Mat4& model);
	void setLights(const math::Vec3 dirLight); // FIXME
	void setMaterial(uint32 index, const StandardMaterial&);
};


} // ns model
} // ns stardazed

#endif
