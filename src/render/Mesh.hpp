// ------------------------------------------------------------------
// render::Mesh - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_MESH_H
#define SD_RENDER_MESH_H

#include "math/Vector.hpp"

#include <array>
#include <vector>

namespace stardazed {
namespace render {


// exposition only, no real support for Tri32 yet
using Tri16 = std::array<uint16_t, 3>;
using Tri32 = std::array<uint32_t, 3>;

using Tri = Tri16;


enum class VertexWinding {
	Clockwise,
	CounterClockwise
};


struct Mesh {
	// mandatory fields
	std::vector<math::Vec3> vertexes, vertexNormals;
	std::vector<render::Tri> faces;
	VertexWinding winding = VertexWinding::CounterClockwise;
	
	// optional fields
	std::vector<math::Vec3> vertexTangents;
	std::vector<math::Vec2> vertexUVs;
	
	// default normal and tangent calculation, will assert on missing vertexes or faces
	void calcVertexNormals();
	void calcVertexTangents();
	
	// observers
	bool isMinimallyComplete() const {
		return ! (vertexes.empty() || vertexNormals.empty() || faces.empty());
	}

	bool hasTangents() const {
		return ! vertexTangents.empty();
	}
	
	bool hasUVs() const {
		return ! vertexUVs.empty();
	}
	
	bool isInternallyConsistent() const {
		bool ok = isMinimallyComplete() && vertexNormals.size() == vertexes.size();
		if (hasTangents())
			ok &= vertexTangents.size() == vertexes.size();
		if (hasUVs())
			ok &= vertexUVs.size() == vertexes.size();
		return ok;
	}
};


} // ns render
} // ns stardazed

#endif
