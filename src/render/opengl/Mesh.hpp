// ------------------------------------------------------------------
// render::opengl::Mesh - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_OPENGLMESH_H
#define SD_RENDER_OPENGLMESH_H

#include "system/Config.hpp"
#include "util/ConceptTraits.hpp"

#include "render/common/Mesh.hpp"
#include "render/opengl/Buffer.hpp"

namespace stardazed {
namespace render {


class Mesh {
	GLsizei drawCount;
	GLuint vao_ {0};
	GLenum winding_;
	
	math::AABB aabb_;

	GLBuffer<math::Vec3> vertexBuffer_, vertexNormalBuffer_;
	GLBuffer<render::Tri, GL_ELEMENT_ARRAY_BUFFER> faceBuffer_;

	GLBuffer<math::Vec3> vertexTangentBuffer_;
	GLBuffer<math::Vec2> vertexUVBuffer_;

public:
	Mesh(const MeshDescriptor&);
	~Mesh();

	virtual math::AABB aabb() const;
	virtual void draw() const;
};


} // ns render
} // ns stardazed

#endif
