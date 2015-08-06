// ------------------------------------------------------------------
// scene::MeshRenderer - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_SCENE_MESHRENDERER_H
#define SD_SCENE_MESHRENDERER_H

#include "system/Config.hpp"
#include "container/MultiArrayBuffer.hpp"
#include "math/Matrix.hpp"
#include "render/common/RenderPass.hpp"
#include "scene/Entity.hpp"

namespace stardazed {
namespace scene {


struct Entity;
	
	
struct ProjectionSetup {
	math::Mat4 projMat, viewMat;
	
	ProjectionSetup(const math::Mat4& projectionMatrix, const math::Mat4& viewMatrix)
	: projMat { projectionMatrix }
	, viewMat { viewMatrix }
	{}
};


struct Renderable {
	virtual ~Renderable() = default;
	virtual void render(render::RenderPass&, const ProjectionSetup&, const Entity&) const = 0;
};


struct MeshRendererDescriptor {
	bool castShadows = false;
	bool receiveShadows = false;
	uint32 materialIndex = 0;
	Renderable* renderable = nullptr;
};


class MeshRendererC {
	using Instance = scene::Instance<MeshRendererC>;
	
public:
	container::MultiArrayBuffer<
		bool, // castShadows
		bool, // receiveShadows
		uint32, // materialIndex
		Renderable*
	> instanceData_;
	
	bool* castShadowsBase_;
	bool* receiveShadowsBase_;
	uint32* materialIndexBase_;
	Renderable** renderableBase_;

	void rebase() {
		castShadowsBase_ = instanceData_.elementsBasePtr<0>();
		receiveShadowsBase_ = instanceData_.elementsBasePtr<1>();
		materialIndexBase_ = instanceData_.elementsBasePtr<2>();
		renderableBase_ = instanceData_.elementsBasePtr<3>();
	}

public:
	MeshRendererC(memory::Allocator& allocator);

	// -- shared Component `interface`
	uint32 count() const { return instanceData_.count(); }
	Instance create(const MeshRendererDescriptor& desc);

	// -- single instance data access
	bool castsShadows(Instance h) const { return castShadowsBase_[h.ref]; }
	bool receivesShadows(Instance h) const { return receiveShadowsBase_[h.ref]; }
	uint32 materialIndex(Instance h) const { return materialIndexBase_[h.ref]; }
	Renderable* renderable(Instance h) const { return renderableBase_[h.ref]; }
	
	void setCastsShadows(Instance, bool newCastShadows);
	void setReceivesShadows(Instance, bool newReceivesShadows);
	void setMaterialIndex(Instance, uint32 newMaterialIndex);
	void setRenderer(Instance, Renderable& newRenderer);
};


} // ns scene
} // ns stardazed

#endif
