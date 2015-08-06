// ------------------------------------------------------------------
// scene::MeshRenderer - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#include "scene/MeshRenderer.hpp"

namespace stardazed {
namespace scene {


MeshRendererC::MeshRendererC(memory::Allocator& allocator)
: instanceData_(allocator, 1024)
{
	rebase();
}


auto MeshRendererC::create(const MeshRendererDescriptor& desc) -> Instance {
	if (__builtin_expect(instanceData_.extend() == container::InvalidatePointers::Yes, 0)) {
		rebase();
	}
	
	Instance h { instanceData_.backIndex() };

	castShadowsBase_[h.ref] = desc.castShadows;
	receiveShadowsBase_[h.ref] = desc.receiveShadows;
	renderableBase_[h.ref] = desc.renderable;
	
	return h;
}


} // ns scene
} // ns stardazed
