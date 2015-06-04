// ------------------------------------------------------------------
// render::opengl::Mesh.cpp - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#include "render/opengl/Mesh.hpp"

namespace stardazed {
namespace render {


constexpr GLenum glTypeForIndexElementType(IndexElementType iet) {
	switch (iet) {
		case IndexElementType::UInt32: return GL_UNSIGNED_INT;
		case IndexElementType::UInt16: return GL_UNSIGNED_SHORT;
		case IndexElementType::UInt8:  return GL_UNSIGNED_BYTE;
	}
}


constexpr GLenum glTypeForVertexField(VertexField vf) {
	switch (vf) {
		case VertexField::Undefined:
			assert(!"Invalid VertexField");
			return GL_NONE;
			
		case VertexField::Float:
		case VertexField::Floatx2:
		case VertexField::Floatx3:
		case VertexField::Floatx4:
			return GL_FLOAT;

		case VertexField::Halfx2:
		case VertexField::Halfx3:
		case VertexField::Halfx4:
			return GL_HALF_FLOAT;

		case VertexField::UInt32:
		case VertexField::UInt32x2:
		case VertexField::UInt32x3:
		case VertexField::UInt32x4:
			return GL_UNSIGNED_INT;
			
		case VertexField::SInt32:
		case VertexField::SInt32x2:
		case VertexField::SInt32x3:
		case VertexField::SInt32x4:
			return GL_INT;
			
		case VertexField::UInt16x2:
		case VertexField::Norm_UInt16x2:
		case VertexField::UInt16x3:
		case VertexField::Norm_UInt16x3:
		case VertexField::UInt16x4:
		case VertexField::Norm_UInt16x4:
			return GL_UNSIGNED_SHORT;

		case VertexField::SInt16x2:
		case VertexField::Norm_SInt16x2:
		case VertexField::SInt16x3:
		case VertexField::Norm_SInt16x3:
		case VertexField::SInt16x4:
		case VertexField::Norm_SInt16x4:
			return GL_SHORT;
			
		case VertexField::UInt8x2:
		case VertexField::Norm_UInt8x2:
		case VertexField::UInt8x3:
		case VertexField::Norm_UInt8x3:
		case VertexField::UInt8x4:
		case VertexField::Norm_UInt8x4:
			return GL_UNSIGNED_BYTE;

		case VertexField::SInt8x2:
		case VertexField::Norm_SInt8x2:
		case VertexField::SInt8x3:
		case VertexField::Norm_SInt8x3:
		case VertexField::SInt8x4:
		case VertexField::Norm_SInt8x4:
			return GL_BYTE;

		case VertexField::Norm_UInt2_10_10_10Rev:
			return GL_UNSIGNED_INT_10_10_10_2;
		case VertexField::Norm_SInt2_10_10_10Rev:
			return GL_INT_2_10_10_10_REV;
	}
}


Mesh::Mesh() {
	glGenVertexArrays(1, &glVAO_);
}


Mesh::Mesh(const MeshDescriptor& descriptor)
: Mesh()
{
	initWithDescriptor(descriptor);
}


Mesh::~Mesh() {
	if (glVAO_ > 0)
		glDeleteVertexArrays(1, &glVAO_);
}


static void bindSingleAttribute(const PositionedAttribute& attr, size32 stride, uint32 toVAIndex) {
	auto elementCount = vertexFieldElementCount(attr.field);
	auto normalized = vertexFieldIsNormalized(attr.field) ? GL_TRUE : GL_FALSE;
	auto glElementType = glTypeForVertexField(attr.field);
	auto offsetPtr = reinterpret_cast<void*>(attr.offset);

	glEnableVertexAttribArray(toVAIndex);
	glVertexAttribPointer(toVAIndex, elementCount, glElementType, normalized, stride, offsetPtr);
}


static void bindVertexBufferAttributes(const VertexBuffer& vb, uint32 startBoundIndex) {
	size32 attrCount = vb.attributeCount(),
		   stride = vb.strideBytes();
	
	assert(startBoundIndex + attrCount <= maxVertexAttributes());
	
	for (size32 attrIndex = 0; attrIndex < attrCount; ++attrIndex) {
		auto attr = vb.attrByIndex(attrIndex);
		assert(attr);
		bindSingleAttribute(*attr, stride, attrIndex + startBoundIndex);
	}
}


void Mesh::initWithDescriptor(const MeshDescriptor& desc) {
	glBindVertexArray(glVAO_);
	
	// -- reserve space in the buffers vector for all buffer objects
	uint32 bufferCount = size32_cast(desc.vertexBindings.size());
	if (desc.indexBinding.indexBuffer) ++bufferCount;
	buffers_.reserve(bufferCount);

	for (const auto& vertexBinding : desc.vertexBindings) {
		assert(vertexBinding.vertexBuffer);

		// -- allocate sized attribute buffer
		buffers_.emplace_back(BufferRole::VertexAttribute, vertexBinding.updateFrequency, vertexBinding.clientAccess);
		auto& buffer = buffers_.back();
		buffer.allocateFromVertexBuffer(*vertexBinding.vertexBuffer);

		// -- configure our VAO attributes with the attrs found in the current vertex buffer
		buffer.bind();
		bindVertexBufferAttributes(*vertexBinding.vertexBuffer, vertexBinding.baseAttributeIndex);
	}
	
	if (desc.indexBinding.indexBuffer) {
		// -- allocate sized index buffer
		buffers_.emplace_back(BufferRole::VertexIndex, desc.indexBinding.updateFrequency, desc.indexBinding.clientAccess);
		auto& indexBuffer = buffers_.back();
		indexBuffer.allocateFromIndexBuffer(*desc.indexBinding.indexBuffer);
		
		// -- bind index buffer to VAO
		indexBuffer.bind();
	}
	
	glBindVertexArray(0);
}


} // ns render
} // ns stardazed
