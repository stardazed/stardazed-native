// ------------------------------------------------------------------
// geom::Generators - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_GEOM_GENERATORS_H
#define SD_GEOM_GENERATORS_H

#include "system/Config.hpp"
#include "math/Angle.hpp"
#include "render/common/Mesh.hpp"

namespace stardazed {
namespace geom {
namespace gen {


//render::MeshDescriptor plane();

render::MeshDescriptor cube(const float diameter = 1.0f);

//render::MeshDescriptor arc(float minRadius, float maxRadius, int radiusSteps,
//						   math::Angle fromAng, math::Angle toAng, int angleSteps);

//render::MeshDescriptor sphere(const int rows = 20, const int segs = 30, const float radius = 1.0f,
//							  const float sliceFrom = 0.0f, const float sliceTo = 1.0f);


} // ns gen
} // ns geom
} // ns stardazed


#endif
