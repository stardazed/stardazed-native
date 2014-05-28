// ------------------------------------------------------------------
// geom::Mesh - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_GEOM_MESH_H
#define SD_GEOM_MESH_H

#include "math/Vector.hpp"

#include <array>
#include <vector>


namespace stardazed {
namespace geom {


using Triangle = std::array<uint16_t, 3>;
using HiTriangle = std::array<uint32_t, 3>;


class Mesh {
public:
	std::vector<Triangle> faces;
	std::vector<math::Vec3> vertexes, vertexNormals, vertexTangents;
	
	void calcVertexNormals();
	void calcVertexTangents();
};


} // ns geom
} // ns stardazed


#endif
