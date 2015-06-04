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


render::MeshData& translate(render::MeshData& mesh, const math::Vec3& globalDelta);

render::MeshData& rotate(render::MeshData& mesh, const math::Quat& rotation);

render::MeshData combine(const render::MeshData& m1, const render::MeshData& m2);


} // ns geom
} // ns stardazed


#endif
