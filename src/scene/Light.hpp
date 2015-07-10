// ------------------------------------------------------------------
// scene::Light - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_SCENE_LIGHT_H
#define SD_SCENE_LIGHT_H

#include "system/Config.hpp"
#include "math/Angle.hpp"
#include "math/Vector.hpp"

namespace stardazed {
namespace scene {


enum class LightType : int {
	Directional,
	Point,
	Spot
};


struct LightDescriptor {
	LightType type = LightType::Directional;
	math::Vec3 colour = math::Vec3::one();
	float intensity = 0;
	float range = 0;
	math::Angle spotAngle = math::Degrees{0};
};


LightDescriptor makeDirectionalLightDesc(const math::Vec3& colour, float intensity);
LightDescriptor makePointLightDesc(const math::Vec3& colour, float intensity, float range);
LightDescriptor makeSpotLightDesc(const math::Vec3& colour, float intensity, float range, math::Angle cutoff);


class Light {
public:
	Light();
	
	struct Handle { uint32 ref; };
	
	Handle create(const LightDescriptor&);
};


} // ns scene
} // ns stardazed

#endif
