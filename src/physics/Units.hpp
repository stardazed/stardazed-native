// ------------------------------------------------------------------
// physics::Units - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_PHYSICS_UNITS_H
#define SD_PHYSICS_UNITS_H

#include "system/Config.hpp"
#include "math/Vector.hpp"

namespace stardazed {
namespace physics {


using Time = double;


//   ___             _            _
//  / __|___ _ _  __| |_ __ _ _ _| |_ ___
// | (__/ _ \ ' \(_-<  _/ _` | ' \  _(_-<
//  \___\___/_|\_/__/\__\__,_|_|\_\__/__/
//

constexpr math::Vec3 earthGravity() { return { 0, -9.80665, 0 }; }
constexpr math::Vec3 moonGravity() { return { 0, -1.62519, 0 }; }

constexpr float earthSeaLevelAirDensity() { return 1.2250; } // at 15°C


} // ns physics
} // ns stardazed

#endif
