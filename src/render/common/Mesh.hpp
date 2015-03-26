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
#include "render/common/TriangleBuffer.hpp"

#include <vector>
#include <array>

namespace stardazed {
namespace render {


class MeshDescriptor {
public:
	MeshDescriptor(const AttributeList&);

	VertexBuffer vertexBuffer;
	TriangleBuffer faces;

	// derived data generation
	math::AABB calcAABB() const;
	void genVertexNormals();
	void genVertexTangents();
};


} // ns render
} // ns stardazed

#endif
