// ------------------------------------------------------------------
// scene::Node - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "math/Vector.hpp"
#include "math/Quaternion.hpp"
#include "render/Mesh.hpp"
#include "render/Material.hpp"

namespace stardazed {
namespace scene {


class Node {
public:
	math::Vec3 position;
	math::Quat orientation;
	
	render::Mesh* mesh;
	render::Material* material;
};


} // ns scene
} // ns stardazed

#endif
