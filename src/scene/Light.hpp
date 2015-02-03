// ------------------------------------------------------------------
// scene::Light - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_SCENE_LIGHT_H
#define SD_SCENE_LIGHT_H

#include "system/Config.hpp"
#include "scene/Entity.hpp"

#include "render/common/Light.hpp"

namespace stardazed {
namespace scene {
	
	
class Light {
	Entity& entity_;
	render::LightDescriptor descriptor_;
	
public:
	Light(Entity&, render::LightDescriptor);
	
	// -- observers
	Entity& entity() { return entity_; }
	const Entity& entity() const { return entity_; }
	
	Transform& transform() { return entity_.transform; }
	const Transform& transform() const { return entity_.transform; }
	
	const render::LightDescriptor descriptor() const { return descriptor_; }
};
	
	
} // ns scene
} // ns stardazed

#endif
