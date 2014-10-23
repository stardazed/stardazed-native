// ------------------------------------------------------------------
// render::Mesh - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_MESH_H
#define SD_RENDER_MESH_H

#include "system/Config.hpp"
#include "render/common/Buffer.hpp"
#include "math/Vector.hpp"
#include "math/AABB.hpp"

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


struct MeshDescriptor {
	BufferFormat format;

	// faces
	std::vector<render::Tri> faces;
	VertexWinding winding = VertexWinding::CounterClockwise;

	// derived data generation
	void calcVertexNormals();
	void calcVertexTangents();
	
	math::AABB calcAABB() const;
	

	// observers
	bool isMinimallyComplete() const;
	bool isInternallyConsistent() const;
};


} // ns render
} // ns stardazed

#endif
