// ------------------------------------------------------------------
// render::OpenGL - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_OPENGL_H
#define SD_RENDER_OPENGL_H

#include "system/DetectPlatform.hpp"

// include platform-specific OpenGL headers
#if SD_PLATFORM_OSX
#	include <OpenGL/gl3.h>
#	include <OpenGL/gl3ext.h>
#else
#	error "No OpenGL support yet for this platform"
#endif


// GLName is a move-only container type for OpenGL "names"
// Given that names are a unique reference to a resource that
// needs to be disposed of, this guards the name and ensures
// it can only be moved, not casually copied.

class GLName {
	GLuint name_ = 0;

public:
	constexpr GLName() = default;
	explicit constexpr GLName(GLuint name) : name_{name} {}
	
	GLName(GLName&& from)
	: name_{ from.name_ }
	{ from.name_ = 0; }
	
	GLName& operator=(GLName&& from) {
		name_ = from.name_;
		from.name_ = 0;
		return *this;
	}
	
	constexpr GLuint& name() { return name_; }
	constexpr GLuint name() const { return name_; }
	
	constexpr operator bool() const { return name_ != 0; }
	constexpr operator GLuint() const { return name_; }
};



/*
 template <typename T>
 void setDefaultAttribPointer(GLuint attribIndex);
 
 template <>
 void setDefaultAttribPointer<math::Vec3>(GLuint attribIndex) {
 glVertexAttribPointer(attribIndex, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
 }
 
 
 template <typename T, GLenum Type = GL_ARRAY_BUFFER>
 class ArrayBuffer {
 mutable GLuint name {0};
 
 public:
 std::vector<T> elements;
 
 ~ArrayBuffer() {
 if (name)
 glDeleteBuffers(1, &name);
 name = 0;
 }
 
 void bind() const {
 glBindBuffer(Type, name);
 }
 
 void upload() const {
 glGenBuffers(1, &name);
 glBindBuffer(Type, name);
 glBufferData(Type, elements.size() * sizeof(T), elements.data(), GL_STATIC_DRAW);
 glBindBuffer(Type, 0);
 }
 
 void makeVAAttribute(GLuint attribIndex) const {
 glBindBuffer(Type, name);
 glEnableVertexAttribArray(attribIndex);
 setDefaultAttribPointer<T>(attribIndex);
 glBindBuffer(Type, 0);
 }
 };
 
 
 class Model {
 GLuint vao, attribIndex {0};
 const geom::Mesh mesh;
 GLsizei faceCount;
 
 public:
 Model(geom::Mesh mesh_);
 ~Model();
 
 void draw();
 };
 
 Model::Model(geom::Mesh mesh_)
 : mesh(std::move(mesh_))
 , faceCount{ static_cast<GLsizei>(mesh.faces.size()) }
 {
 glGenVertexArrays(1, &vao);
 
 glBindVertexArray(vao);
 mesh.vertexes.upload();
 mesh.vertexes.makeVAAttribute(attribIndex++);
 mesh.vertexNormals.upload();
 mesh.vertexNormals.makeVAAttribute(attribIndex++);
 mesh.faces.upload();
 glBindVertexArray(0);
 }
 
 Model::~Model() {
 glDeleteVertexArrays(1, &vao);
 vao = 0;
 }
 
 void Model::draw() {
 glBindVertexArray(vao);
 mesh.faces.bind();
 glDrawElements(GL_TRIANGLES, faceCount * 3, GL_UNSIGNED_SHORT, nullptr); // FIXME: move to Mesh
 glBindVertexArray(0);
 }
 */


#endif
