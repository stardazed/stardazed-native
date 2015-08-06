// ------------------------------------------------------------------
// render::MeshData - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_MESH_H
#define SD_RENDER_MESH_H

#include "system/Config.hpp"
#include "math/Vector.hpp"
#include "container/Array.hpp"
#include "render/common/VertexBuffer.hpp"
#include "render/common/IndexBuffer.hpp"

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

	BufferClientAccess clientAccess = BufferClientAccess::WriteOnly;
	BufferUpdateFrequency updateFrequency = BufferUpdateFrequency::Never;
};


struct IndexBufferBinding {
	const IndexBuffer* indexBuffer = nullptr;
	
	BufferClientAccess clientAccess = BufferClientAccess::WriteOnly;
	BufferUpdateFrequency updateFrequency = BufferUpdateFrequency::Never;
};


struct FaceGroup {
	uint32 fromFaceIx, faceCount;
	uint32 materialIx; // mesh-local index (starting at 0); representation of Materials is external to MeshData
};


struct MeshDescriptor {
	Array<VertexBufferBinding> vertexBindings;
	IndexBufferBinding indexBinding;
	Array<FaceGroup> faceGroups;
};


class MeshData {
public:
	MeshData() = default;
	MeshData(const VertexAttributeList&);

	Array<VertexBuffer> vertexBuffers;
	IndexBuffer indexBuffer;
	Array<FaceGroup> faceGroups;

	VertexBuffer& primaryVertexBuffer();
	MeshDescriptor defaultDescriptor() const;

	// derived vertex data generation
	void genVertexNormals();
	void genVertexTangents();
};


class Mesh;


} // ns render
} // ns stardazed

#endif
