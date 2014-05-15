// ------------------------------------------------------------------
// geom::Primitives - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_GEOM_PRIMITIVES_H
#define SD_GEOM_PRIMITIVES_H


#include "math/Vector.hpp"

#include <array>
#include <vector>


namespace stardazed {
namespace geom {


using Triangle = std::array<uint16_t, 3>;


class Mesh {
public:
	std::vector<Triangle> faces;
	std::vector<math::Vec3> vertexes, vertexNormals;
	
	void calcVertexNormals();
};


} // ns geom
} // ns stardazed


#endif
