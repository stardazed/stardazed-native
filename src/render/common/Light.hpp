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
	

enum class ShadowType {
	None,
	Hard,
	Soft
};


enum class ShadowQuality {
	Auto,
	Low,      // 1/8
	Medium,   // 1/4
	High,     // 1/2
	Full      // 1/1
};


enum class LightType : uint {
	Directional,
	Point,
	Spot
};


struct LightDescriptor {
	math::Vec3 colour = math::Vec3::one();
	float intensity = 1;
	math::Vec3 worldPosition = math::Vec3::zero();
	float range = 0;
	math::Vec3 orientation { 0, -1, 0 };
	math::Angle spotAngle = math::Degrees{0};
};


} // ns render
} // ns stardazed

#endif
