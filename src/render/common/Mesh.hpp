// ------------------------------------------------------------------
// render::Mesh - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_MESH_H
#define SD_RENDER_MESH_H

#include "system/Config.hpp"
#include "math/Vector.hpp"
#include "math/AABB.hpp"
#include "render/common/VertexBuffer.hpp"
#include "render/common/IndexBuffer.hpp"

#include <vector>
#include <array>

namespace stardazed {
namespace render {


struct VertexBufferBinding {
	VertexBuffer* vertexBuffer = nullptr;
	uint32 baseAttributeIndex = 0;
};


struct MeshDescriptor {
	std::vector<VertexBufferBinding> vertexBuffers;
	IndexBuffer* indices = nullptr;
};


class Mesh {
public:
	Mesh(const VertexAttributeList&);

	VertexBuffer vertexBuffer;
	IndexBuffer indexBuffer;

	// derived data generation
	math::AABB calcAABB() const;
	void genVertexNormals();
	void genVertexTangents();
};


} // ns render
} // ns stardazed

#endif
