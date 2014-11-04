// ------------------------------------------------------------------
// render::Mesh - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_MESH_H
#define SD_RENDER_MESH_H

#include "system/Config.hpp"
#include "math/Vector.hpp"
#include "math/AABB.hpp"
#include "render/common/VertexBuffer.hpp"

#include <array>
#include <vector>

namespace stardazed {
namespace render {


enum class VertexWinding {
	Clockwise,
	CounterClockwise
};


class MeshDescriptor {
public:
	MeshDescriptor(const AttributeList&);
	VertexBuffer vertexBuffer;

	std::vector<render::Tri> faces;
	VertexWinding winding = VertexWinding::CounterClockwise;

	math::AABB calcAABB() const;
	
	// derived data generation
	void genVertexNormals();
};


} // ns render
} // ns stardazed

#endif
