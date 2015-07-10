// ------------------------------------------------------------------
// scene::Light.cpp - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#include "scene/Light.hpp"

namespace stardazed {
namespace scene {


Light::Light()
: instanceData_{ memory::SystemAllocator::sharedInstance(), 512 }
{
	rebase();
}


void Light::rebase() {
	typeBase_ = instanceData_.elementsBasePtr<0>();
	enabledBase_ = instanceData_.elementsBasePtr<1>();
	colourBase_ = instanceData_.elementsBasePtr<2>();
	intensityBase_ = instanceData_.elementsBasePtr<3>();
	rangeBase_ = instanceData_.elementsBasePtr<4>();
	cutoffBase_ = instanceData_.elementsBasePtr<5>();
}


Light::Handle Light::append(const LightDescriptor& desc) {
	if (__builtin_expect(instanceData_.append() == container::InvalidatePointers::Yes, 0)) {
		rebase();
	}

	Handle h { nextRef_++ };
	setType(h, desc.type);
	setEnabled(h, true);
	setColour(h, desc.colour);
	setIntensity(h, desc.intensity);
	
	if (desc.type != LightType::Directional) {
		setRange(h, desc.range);
		if (desc.type == LightType::Spot)
			setCutoff(h, desc.spotCutoff);
	}
	
	return h;
}


} // ns scene
} // ns stardazed
