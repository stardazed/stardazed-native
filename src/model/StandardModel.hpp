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
#include "container/HashMap.hpp"

#include "render/common/Mesh.hpp"
#include "render/common/Texture.hpp"
#include "render/common/Pipeline.hpp"
#include "render/common/RenderPass.hpp"
#include "render/RenderContext.hpp"

#include "render/opengl/Buffer.hpp"

#include "scene/Entity.hpp"
#include "scene/Transform.hpp"
#include "scene/RendererShared.hpp"


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


class StandardMaterialBuffer {
	render::Buffer materialsConstBuffer_;
	uint32 materialsPerBlock_;
	uint32 rangeBlockSizeBytes_, rangeBlockSizeBytesAligned_;
	uint32 nextIndex_, maxIndex_;
	uint32 uniformOffsetAlignment_;
	uint32 firstBoundMaterialIndex_ = 0;
	
public:
	StandardMaterialBuffer();
	SD_NOCOPYORMOVE_CLASS(StandardMaterialBuffer)

	struct Index { uint32 index; };
	
	Index append(const StandardMaterialDescriptor&);
	void appendMultiple(const StandardMaterialDescriptor* base, uint32 count, Index* outIndexesBase);
	
	void mapMaterialAtBindPoint(Index material, uint32 bindPoint);
	uint32 firstBoundMaterialIndex() const { return firstBoundMaterialIndex_; };
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
	const render::Pipeline& pipeline() const { return *pipeline_; }
	
	void setMatrices(const math::Mat4& projection, const math::Mat4& view, const math::Mat4& model);
	void setLights(const math::Vec3 dirLight); // FIXME
	void setMaterial(StandardMaterialBuffer::Index/* , const StandardMaterialDescriptor&*/);
};


//  ___ _                _             _ __  __         _     _ __  __
// / __| |_ __ _ _ _  __| |__ _ _ _ __| |  \/  |___  __| |___| |  \/  |__ _ _ _
// \__ \  _/ _` | ' \/ _` / _` | '_/ _` | |\/| / _ \/ _` / -_) | |\/| / _` | '_|
// |___/\__\__,_|_||_\__,_\__,_|_| \__,_|_|  |_\___/\__,_\___|_|_|  |_\__, |_|
//                                                                    |___/
struct StandardModelDescriptor {
	render::Mesh* mesh;
	Array<StandardMaterialDescriptor> materials;
	Array<render::FaceGroup> faceGroups;
};


class StandardModelManager {
public:
	using Instance = scene::Instance<StandardModelManager>;

private:
	StandardShader stdShader_;
	StandardMaterialBuffer stdMaterialBuffer_;
	
	Array<StandardMaterialBuffer::Index> materialIndexes_;
	Array<render::FaceGroup> faceGroups_;

	struct IndexRange {
		uint first, count;
	};

	container::MultiArrayBuffer<
		render::Mesh*,
		IndexRange,  // range within materialIndexes_
		IndexRange   // range within faceGroups_
	> instanceData_;

	HashMap<scene::Entity, Instance> entityMap_;

public:
	StandardModelManager(render::RenderContext&);

	Instance create(const StandardModelDescriptor&);
	void linkEntityToModel(scene::Entity, Instance);
	Instance forEntity(scene::Entity) const;

	void render(render::RenderPass& renderPass, const scene::ProjectionSetup& proj, const math::Mat4& modelMatrix, Instance instance);
};


} // ns model
} // ns stardazed

#endif
