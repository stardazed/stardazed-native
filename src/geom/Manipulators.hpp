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


render::MeshDescriptor& translate(render::MeshDescriptor& mesh, const math::Vec3& globalDelta);

render::MeshDescriptor& rotate(render::MeshDescriptor& mesh, const math::Quat& rotation);

render::MeshDescriptor combine(const render::MeshDescriptor& m1, const render::MeshDescriptor& m2);


} // ns geom
} // ns stardazed


#endif
