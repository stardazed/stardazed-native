// ------------------------------------------------------------------
// render::OpenGLMesh - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_OPENGLMESH_H
#define SD_RENDER_OPENGLMESH_H

#include "system/Config.hpp"
#include "util/ConceptTraits.hpp"

#include "render/common/Mesh.hpp"
#include "render/opengl/OpenGLBuffer.hpp"

namespace stardazed {
namespace render {


class Mesh {
	GLsizei drawCount;
	GLuint vao_ {0};
	GLenum winding_;
	
	math::AABB aabb_;

	OpenGLBuffer<math::Vec3> vertexBuffer_, vertexNormalBuffer_;
	OpenGLBuffer<render::Tri, GL_ELEMENT_ARRAY_BUFFER> faceBuffer_;

	OpenGLBuffer<math::Vec3> vertexTangentBuffer_;
	OpenGLBuffer<math::Vec2> vertexUVBuffer_;

public:
	Mesh(const MeshDescriptor&);
	~Mesh();

	virtual math::AABB aabb() const;
	virtual void draw() const;
};


} // ns render
} // ns stardazed

#endif
