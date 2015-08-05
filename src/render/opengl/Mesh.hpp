// ------------------------------------------------------------------
// render::opengl::Mesh - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_OPENGL_MESH_H
#define SD_RENDER_OPENGL_MESH_H

#include "system/Config.hpp"
#include "container/Array.hpp"
#include "util/ConceptTraits.hpp"
#include "render/common/Mesh.hpp"
#include "render/opengl/Buffer.hpp"

namespace stardazed {
namespace render {


class Mesh {
	GLResource resource_ {};

	Array<Buffer> buffers_;
	Array<FaceGroup> faceGroups_;

	GLenum glPrimitiveType_ = GL_NONE;
	GLenum glIndexElementType_ = GL_NONE;
	size32 indexElementSizeBytes_ = 0;

public:
	Mesh();
	Mesh(const MeshDescriptor&);
	~Mesh();
	SD_DEFAULT_MOVE_OPS(Mesh)

	// FIXME: hmmmmm
	void initWithDescriptor(const MeshDescriptor&);
	
	// -- observers
	Buffer* vertexBufferAtIndex(uint32);
	Buffer* indexBuffer();
	
	bool hasIndexBuffer() const { return glIndexElementType_ != GL_NONE; }
	GLenum glPrimitiveType() const { return glPrimitiveType_; }
	GLenum glIndexElementType() const { return glIndexElementType_; }
	size32 indexElementSizeBytes() const { return indexElementSizeBytes_; }

	GLuint name() const { return resource_.name(); }

	void bind() const {
		glBindVertexArray(name());
	}
};


} // ns render
} // ns stardazed


#endif
