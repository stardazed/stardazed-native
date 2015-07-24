// ------------------------------------------------------------------
// scene::MeshRenderer - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_SCENE_MESHRENDERER_H
#define SD_SCENE_MESHRENDERER_H

#include "system/Config.hpp"
#include "container/MultiElementArrayBuffer.hpp"
#include "math/Matrix.hpp"
#include "render/common/RenderPass.hpp"

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
	container::MultiElementArrayBuffer<
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

	struct Handle { uint32 ref; };

	// -- shared Component `interface`
	uint32 count() const { return instanceData_.count(); }
	Handle append(const MeshRendererDescriptor& desc);

	// -- single instance data access
	bool castsShadows(Handle h) const { return castShadowsBase_[h.ref]; }
	bool receivesShadows(Handle h) const { return receiveShadowsBase_[h.ref]; }
	uint32 materialIndex(Handle h) const { return materialIndexBase_[h.ref]; }
	Renderable* renderable(Handle h) const { return renderableBase_[h.ref]; }
	
	void setCastsShadows(Handle h, bool newCastShadows);
	void setReceivesShadows(Handle h, bool newReceivesShadows);
	void setMaterialIndex(Handle h, uint32 newMaterialIndex);
	void setRenderer(Handle h, Renderable& newRenderer);
};


} // ns scene
} // ns stardazed

#endif
