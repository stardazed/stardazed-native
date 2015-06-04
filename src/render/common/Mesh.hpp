// ------------------------------------------------------------------
// render::MeshData - stardazed
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

namespace stardazed {
namespace render {


enum class BufferClientAccess {
	None,
	ReadOnly,
	WriteOnly,
	ReadWrite
};


enum class BufferUpdateFrequency {
	Never,
	Occassionally,
	Frequently
};


struct VertexBufferBinding {
	const VertexBuffer* vertexBuffer = nullptr;
	uint32 baseAttributeIndex = 0;

	BufferClientAccess clientAccess = BufferClientAccess::None;
	BufferUpdateFrequency updateFrequency = BufferUpdateFrequency::Never;
};


struct IndexBufferBinding {
	const IndexBuffer* indexBuffer = nullptr;
	
	BufferClientAccess clientAccess = BufferClientAccess::None;
	BufferUpdateFrequency updateFrequency = BufferUpdateFrequency::Never;
};


struct MeshDescriptor {
	std::vector<VertexBufferBinding> vertexBindings;
	IndexBufferBinding indexBinding;
};


class MeshData {
public:
	MeshData() = default;
	MeshData(const VertexAttributeList&);

	std::vector<VertexBuffer> vertexBuffers;
	IndexBuffer indexBuffer;

	VertexBuffer& primaryVertexBuffer();
	MeshDescriptor defaultDescriptor() const;

	// derived vertex data generation
	void genVertexNormals();
	void genVertexTangents();
};


} // ns render
} // ns stardazed

#endif
