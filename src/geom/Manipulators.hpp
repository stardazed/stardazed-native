// ------------------------------------------------------------------
// geom::Manipulators - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_GEOM_MANIPULATORS_H
#define SD_GEOM_MANIPULATORS_H

#include "system/Config.hpp"
#include "math/Angle.hpp"
#include "math/Vector.hpp"
#include "math/Quaternion.hpp"
#include "render/common/Mesh.hpp"

namespace stardazed {
namespace geom {


render::Mesh& translate(render::Mesh& mesh, const math::Vec3& globalDelta);

render::Mesh& rotate(render::Mesh& mesh, const math::Quat& rotation);

render::Mesh combine(const render::Mesh& m1, const render::Mesh& m2);


} // ns geom
} // ns stardazed


#endif
