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
	Directional
};


struct LightAttenuation {
	float constant = 0;
	float linear = 0;
	float exponential = 0;
};


struct LightDescriptor {
	LightType type;
	math::Vec3 colour {};
	float intensity = 1;
};


constexpr LightDescriptor makeLightDescDirectional(
	const math::Vec3& colour,
	float intensity = 1)
{
	return {
		LightType::Directional,
		colour,
		intensity
	};
}


} // ns render
} // ns stardazed

#endif
