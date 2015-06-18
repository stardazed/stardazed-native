// ------------------------------------------------------------------
// physics::PhysicMaterial - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_PHYSICS_PHYSICMATERIAL_H
#define SD_PHYSICS_PHYSICMATERIAL_H

#include "system/Config.hpp"

namespace stardazed {
namespace physics {


enum class PropertyCombiner : int {
	Average,
	Minimum,
	Maximum,
	Multiply
};


struct PhysicMaterial {
	float bounciness = 0;
	PropertyCombiner bounceCombiner = PropertyCombiner::Average;
};


} // ns physics
} // ns stardazed

#endif
