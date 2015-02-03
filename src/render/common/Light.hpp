// ------------------------------------------------------------------
// render::Light - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_LIGHT_H
#define SD_RENDER_LIGHT_H

#include "system/Config.hpp"
#include "math/Angle.hpp"
#include "math/Vector.hpp"

namespace stardazed {
namespace render {
	

enum class LightType : uint32 {
	Directional,
	Point,
	Spot
};


struct LightAttenuation {
	float constant = 0;
	float linear = 0;
	float exponential = 0;
};


struct LightDescriptor {
	math::Vec3 colour;
	LightType type;
	float intensity = 1;

	// Point and Spot only
	LightAttenuation attenuation {};

	// Spot only
	math::Angle spotCutoff = math::Radians{0};
	
	// position and direction are taken from the entity connected to the light
};


constexpr LightDescriptor directionalLight(const math::Vec3& colour,
										   float intensity = 1)
{
	return {
		colour,
		LightType::Directional,
		intensity
	};
}


constexpr LightDescriptor pointLight(const math::Vec3& colour,
									 const LightAttenuation& attenuation,
									 float intensity = 1)
{
	return {
		colour,
		LightType::Point,
		intensity,
		attenuation
	};
}
	
	
constexpr LightDescriptor spotLight(const math::Vec3& colour,
									const LightAttenuation& attenuation,
									math::Angle cutoff,
									float intensity = 1)
{
	return {
		colour,
		LightType::Spot,
		intensity,
		attenuation,
		cutoff
	};
}


} // ns render
} // ns stardazed

#endif
