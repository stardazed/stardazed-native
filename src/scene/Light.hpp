// ------------------------------------------------------------------
// scene::Light - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_SCENE_LIGHT_H
#define SD_SCENE_LIGHT_H

#include "system/Config.hpp"
#include "math/Angle.hpp"
#include "math/Vector.hpp"
#include "container/MultiElementArrayBuffer.hpp"

namespace stardazed {
namespace scene {


enum class LightType : int8 {
	Directional,
	Point,
	Spot
};


struct LightDescriptor {
	LightType type = LightType::Directional;
	math::Vec3 colour = math::Vec3::one();
	float intensity = 0;
	float range = 0;
	math::Angle spotCutoff {};
};


LightDescriptor makeDirectionalLightDesc(const math::Vec3& colour, float intensity);
LightDescriptor makePointLightDesc(const math::Vec3& colour, float intensity, float range);
LightDescriptor makeSpotLightDesc(const math::Vec3& colour, float intensity, float range, math::Angle cutoff);


class Light {
	container::MultiElementArrayBuffer<
		LightType,  // type
		bool8,      // enabled
		math::Vec3, // colour
		float,      // intensity
		float,      // range
		math::Angle // cutoff
	> instanceData_;
	
	LightType* typeBase_ = nullptr;
	bool8* enabledBase_ = nullptr;
	math::Vec3* colourBase_ = nullptr;
	float* intensityBase_ = nullptr;
	float* rangeBase_ = nullptr;
	math::Angle* cutoffBase_ = nullptr;
	
	void rebase();
	
	uint32 nextRef_ = 1;

public:
	Light();
	
	struct Handle { uint32 ref; };
	
	uint32 count() const { return instanceData_.count(); }
	Handle append(const LightDescriptor&);

	// -- single instance data access
	LightType type(Handle h) const { return typeBase_[h.ref]; }
	bool8 enabled(Handle h) const { return enabledBase_[h.ref]; }
	math::Vec3 colour(Handle h) const { return colourBase_[h.ref]; }
	float intensity(Handle h) const { return intensityBase_[h.ref]; }
	float range(Handle h) const { return rangeBase_[h.ref]; }
	math::Angle cutoff(Handle h) const { return cutoffBase_[h.ref]; }

	void setType(Handle h, LightType newType) const { typeBase_[h.ref] = newType; }
	void setEnabled(Handle h, bool8 newEnabled) const { enabledBase_[h.ref] = newEnabled; }
	void setColour(Handle h, const math::Vec3& newColour) const { colourBase_[h.ref] = newColour; }
	void setIntensity(Handle h, float newIntensity) const { intensityBase_[h.ref] = newIntensity; }
	void setRange(Handle h, float newRange) const { rangeBase_[h.ref] = newRange; }
	void setCutoff(Handle h, math::Angle newCutoff) { cutoffBase_[h.ref] = newCutoff; }
};


} // ns scene
} // ns stardazed

#endif
