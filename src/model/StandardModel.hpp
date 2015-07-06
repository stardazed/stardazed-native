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


struct StandardMaterialDescriptor {
	// colours
	math::Vec3 mainColour = math::Vec3::one();        // single colour or tint for albedo
	float pad__ = 1;
	math::Vec3 specularColour = math::Vec3::zero();
	float specularExponent = 0;                       // 0 means no specular

	// textures
	math::Vec2 textureScale = math::Vec2::one();      // scale and offset apply to all textures
	math::Vec2 textureOffset = math::Vec2::zero();

	render::Texture* albedoMap = nullptr;             // nullptr means use mainColour only
	render::Texture* normalMap = nullptr;             // nullptr means no bump
};


class StandardMaterial {
	render::Buffer materialsConstBuffer_;
	uint32 maxMaterialsPerMappedRange_, rangeBlockSizeBytes_;
	uint32 nextIndex_, maxIndex_;
	
public:
	StandardMaterial();
	SD_NOCOPYORMOVE_CLASS(StandardMaterial)

	struct Index { uint32 index; };
	
	Index alloc(const StandardMaterialDescriptor&);
	void allocMultiple(const StandardMaterialDescriptor* base, uint32 count, Index* outIndexesBase);
	
	void mapMaterialAtBindPoint(Index material, uint32 bindPoint);
};


struct FaceGroup {
	uint32 materialIx, fromFaceIx, toFaceIx;          // materialIx is a model-local index (not a StandardMaterial::Index val)
};


struct StandardModelDescriptor {
	render::Mesh* mesh;
	std::vector<StandardMaterialDescriptor> materials;
	std::vector<FaceGroup> faceGroups;
};


class StandardShader;


class StandardModel : public scene::Renderable {
	StandardModelDescriptor descriptor_;
	std::vector<StandardMaterial::Index> materialIndexes_;
	render::Mesh& mesh_;
	StandardShader& shader_;
	
	static StandardMaterial& standardMaterial();

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
	
	void setMatrices(const math::Mat4& projection, const math::Mat4& view, const math::Mat4& model);
	void setLights(const math::Vec3 dirLight); // FIXME
	void setMaterial(StandardMaterial::Index, const StandardMaterialDescriptor&);
};


} // ns model
} // ns stardazed

#endif
