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

#include <type_traits>

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




class MeshRendererC {
	memory::ArenaAllocator arena_;
	container::MultiElementArrayBuffer<
		bool, // castShadows
		bool, // receiveShadows
		Renderable*
	> items_;

public:
	MeshRendererC(memory::Allocator& allocator)
	: arena_(allocator)
	, items_(allocator, 1024)
	{}
	
	~MeshRendererC() {
		auto renderables = items_.elementsBasePtr<2>();
		auto count = items_.count();
		while (count--) {
			(*renderables)->~Renderable();
			++renderables;
		}
	}

	struct Handle { uint32 ref; };

	template <typename Rend>
	Handle append(const Rend& renderable) {
		static_assert(std::is_base_of<Renderable, Rend>::value, "Instance must be a Renderable");

		auto space = static_cast<Renderable*>(arena_.alloc(sizeof(Rend)));
		
		if (std::is_trivially_copy_constructible<Rend>)
			;
		new (space) Renderable{t};

		items_.emplaceBack(space);
		return { count_++ };
	}
	
	template <typename T, typename... Args>
	Handle emplace(Args&&... args) {
		auto space = static_cast<RendererInstance<T>*>(arena_.alloc(sizeof(RendererInstance<T>)));
		new (space) RendererInstance<T>(ConstructInPlace{}, std::forward<Args>(args)...);
		items_.emplaceBack(space);
		return { count_++ };
	}
	
	void run(Handle h, Time t) {
		reinterpret_cast<RendererConcept*>(items_[h.ref])->run_(t);
	}
	
	void runAll(Time t) {
		for (auto b = 0u; b < count_; ++b)
			reinterpret_cast<RendererConcept*>(items_[b])->run_(t);
	}
};


} // ns scene
} // ns stardazed

#endif
