// ------------------------------------------------------------------
// model::StandardModel
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_MODEL_STANDARDMODEL_H
#define SD_MODEL_STANDARDMODEL_H

#include "system/Config.hpp"
#include "math/Vector.hpp"
#include "container/Array.hpp"
#include "container/MultiArrayBuffer.hpp"
#include "render/common/Mesh.hpp"
#include "render/common/Texture.hpp"
#include "render/common/Pipeline.hpp"
#include "render/common/RenderPass.hpp"

#include "render/opengl/Buffer.hpp"

#include "render/RenderContext.hpp"
#include "scene/MeshRenderer.hpp"

#include "scene/Entity.hpp"
#include "scene/Transform.hpp"


namespace stardazed {
namespace model {


//  ___ _                _             _ __  __      _           _      _
// / __| |_ __ _ _ _  __| |__ _ _ _ __| |  \/  |__ _| |_ ___ _ _(_)__ _| |
// \__ \  _/ _` | ' \/ _` / _` | '_/ _` | |\/| / _` |  _/ -_) '_| / _` | |
// |___/\__\__,_|_||_\__,_\__,_|_| \__,_|_|  |_\__,_|\__\___|_| |_\__,_|_|
//
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


class StandardMaterialComponent {
	render::Buffer materialsConstBuffer_;
	uint32 materialsPerBlock_;
	uint32 rangeBlockSizeBytes_, rangeBlockSizeBytesAligned_;
	uint32 nextIndex_, maxIndex_;
	uint32 uniformOffsetAlignment_;
	uint32 firstBoundMaterialIndex_ = 0;
	
public:
	StandardMaterialComponent();
	SD_NOCOPYORMOVE_CLASS(StandardMaterialComponent)

	struct Index { uint32 index; };
	
	Index append(const StandardMaterialDescriptor&);
	void appendMultiple(const StandardMaterialDescriptor* base, uint32 count, Index* outIndexesBase);
	
	void mapMaterialAtBindPoint(Index material, uint32 bindPoint);
	uint32 firstBoundMaterialIndex() const { return firstBoundMaterialIndex_; };
};


//  ___ _                _             _ __  __         _     _
// / __| |_ __ _ _ _  __| |__ _ _ _ __| |  \/  |___  __| |___| |
// \__ \  _/ _` | ' \/ _` / _` | '_/ _` | |\/| / _ \/ _` / -_) |
// |___/\__\__,_|_||_\__,_\__,_|_| \__,_|_|  |_\___/\__,_\___|_|
//
struct FaceGroup {
	uint32 materialIx, fromFaceIx, toFaceIx;  // materialIx is a model-local index (not a StandardMaterial::Index val)
};


struct StandardModelDescriptor {
	render::Mesh* mesh;
	std::vector<StandardMaterialDescriptor> materials;
	std::vector<FaceGroup> faceGroups;
};


class StandardShader;


class StandardModelComponent {
//	StandardModelDescriptor descriptor_;
//	std::vector<StandardMaterialComponent::Index> materialIndexes_;
//	render::Mesh& mesh_;
	StandardShader& stdShader_;
	StandardMaterialComponent& stdMaterialComponent_;
	scene::TransformComponent& transformComponent_;
	
	Array<StandardMaterialComponent::Index> materialIndexes_;
	Array<FaceGroup> faceGroups_;
	container::MultiArrayBuffer<
		render::Mesh*,
		uint,  // index within materialIndexes_
		uint   // index within faceGroups_
	> instanceData_;

public:
	StandardModelComponent(StandardShader&, StandardMaterialComponent&, scene::TransformComponent&);
	
	scene::Handle append(const StandardModelDescriptor&);
//	void render(render::RenderPass&, const scene::ProjectionSetup&, const scene::Entity&) const;
};


//  ___ _                _             _ ___ _            _
// / __| |_ __ _ _ _  __| |__ _ _ _ __| / __| |_  __ _ __| |___ _ _
// \__ \  _/ _` | ' \/ _` / _` | '_/ _` \__ \ ' \/ _` / _` / -_) '_|
// |___/\__\__,_|_||_\__,_\__,_|_| \__,_|___/_||_\__,_\__,_\___|_|
//
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
	void setMaterial(StandardMaterialComponent::Index, const StandardMaterialDescriptor&);
};


} // ns model
} // ns stardazed

#endif
