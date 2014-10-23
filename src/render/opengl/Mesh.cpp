// ------------------------------------------------------------------
// render::opengl::Mesh - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "render/opengl/Mesh.hpp"

namespace stardazed {
namespace render {


enum OpenGLVertexAttribute : GLuint {
	VertexPosition   = 0,
	VertexNormal     = 1,
	VertexTangent    = 2,
	VertexUV         = 3
};



Mesh::Mesh(const MeshDescriptor& mesh)
: drawCount(static_cast<GLsizei>(mesh.faces.size()) * 3)
{
	if (mesh.winding == VertexWinding::Clockwise)
		winding_ = GL_CW;
	else
		winding_ = GL_CCW;
	
	aabb_ = mesh.calcAABB();

	glGenVertexArrays(1, &vao_);
	glBindVertexArray(vao_);
/*
	vertexBuffer_.initialize(mesh.vertexes, GL_STATIC_DRAW);
	vertexBuffer_.assignToVAOAttribute(VertexPosition);

	vertexNormalBuffer_.initialize(mesh.vertexNormals, GL_STATIC_DRAW);
	vertexNormalBuffer_.assignToVAOAttribute(VertexNormal);

	if (mesh.hasTangents()) {
		vertexTangentBuffer_.initialize(mesh.vertexTangents, GL_STATIC_DRAW);
		vertexTangentBuffer_.assignToVAOAttribute(VertexTangent);
	}

	if (mesh.hasUVs()) {
		vertexUVBuffer_.initialize(mesh.vertexUVs, GL_STATIC_DRAW);
		vertexUVBuffer_.assignToVAOAttribute(VertexUV);
	}

	faceBuffer_.initialize(mesh.faces, GL_STATIC_DRAW);
*/
	glBindVertexArray(0);
}


Mesh::~Mesh() {
	if (vao_)
		glDeleteVertexArrays(1, &vao_);
}


math::AABB Mesh::aabb() const {
	return aabb_;
}


void Mesh::draw() const {
	glBindVertexArray(vao_);
//	faceBuffer_.bind();
//	glFrontFace(winding_);
//	glDrawElements(GL_TRIANGLES, drawCount, GL_UNSIGNED_SHORT, nullptr);
	glBindVertexArray(0);
}

		
} // ns render
} // ns stardazed
