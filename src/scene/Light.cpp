// ------------------------------------------------------------------
// scene::Light - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "scene/Light.hpp"

namespace stardazed {
namespace scene {
	

Light::Light(Entity& linkedEntity, render::LightDescriptor descriptor)
: entity_(linkedEntity)
, descriptor_(descriptor)
{
}
	
	
} // ns scene
} // ns stardazed
