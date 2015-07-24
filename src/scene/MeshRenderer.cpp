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


void MeshRendererC::rebase() {
	castShadowsBase_ = items_.elementsBasePtr<0>();
	receiveShadowsBase_ = items_.elementsBasePtr<1>();
	renderableBase_ = items_.elementsBasePtr<2>();
}


Handle append(const MeshRendererDescriptor& desc) {
	if (__builtin_expect(instanceData_.append() == container::InvalidatePointers::Yes, 0)) {
		rebase();
	}
	
	Handle h { instanceData_.backIndex() };

	castShadowsBase_[h.ref] = desc.castShadows;
	receiveShadowsBase_[h.ref] = desc.receiveShadows;
	renderableBase_[h.ref] = desc.renderable;
	
	return h;
}


} // ns scene
} // ns stardazed
