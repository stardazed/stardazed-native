// ------------------------------------------------------------------
// render::opengl::VertexArray.cpp - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "render/opengl/VertexArray.hpp"

#include <cassert>

namespace stardazed {
namespace render {


constexpr GLenum glTypeForElementType(ElementType et) {
	switch (et) {
		case ElementType::UInt8:  return GL_UNSIGNED_BYTE;
		case ElementType::SInt8:  return GL_BYTE;
		case ElementType::UInt16: return GL_UNSIGNED_SHORT;
		case ElementType::SInt16: return GL_SHORT;
		case ElementType::UInt32: return GL_UNSIGNED_INT;
		case ElementType::SInt32: return GL_INT;
			
		case ElementType::Float:  return GL_FLOAT;
		case ElementType::Double: return GL_DOUBLE;
			
		default: assert(false && "No suitable GL type for ElementType");
	}
}


static void bindAttributeImpl(const PositionedAttribute& attr, size32_t stride, uint32_t toVAIndex) {
	glEnableVertexAttribArray(toVAIndex);
	
	auto field = getField(attr);
	auto offsetPtr = reinterpret_cast<void*>(attr.offset);
	
	if (field.type == ElementType::Float)
		glVertexAttribPointer(toVAIndex, field.count, GL_FLOAT, GL_FALSE, stride, offsetPtr);
	else if (field.type == ElementType::Double)
		glVertexAttribLPointer(toVAIndex, field.count, GL_DOUBLE, stride, offsetPtr);
	else
		glVertexAttribIPointer(toVAIndex, field.count, glTypeForElementType(field.type), stride, offsetPtr);
}


void GLVertexArray::bindVertexBufferAttributes(const VertexBuffer& vb, uint32_t startBoundIndex) {
	size32_t attrCount = vb.attributeCount(),
	         stride = vb.itemSizeBytes();
	
	for (size32_t attrIndex = 0; attrIndex < attrCount; ++attrIndex) {
		auto attr = vb.attrByIndex(attrIndex);
		bindAttributeImpl(*attr, stride, attrIndex + startBoundIndex);
	}
}



} // ns render
} // ns stardazed
