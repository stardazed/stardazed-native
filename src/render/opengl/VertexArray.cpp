// ------------------------------------------------------------------
// render::opengl::VertexArray.cpp - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "render/opengl/VertexArray.hpp"

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
		
		case ElementType::SInt2_10_10_10Rev:
		case ElementType::SNormInt2_10_10_10Rev:
			return GL_INT_2_10_10_10_REV;
		case ElementType::UInt2_10_10_10Rev:
		case ElementType::UNormInt2_10_10_10Rev:
			return GL_UNSIGNED_INT_2_10_10_10_REV;
			
		default: assert(false && "No suitable GL type for ElementType");
	}
}


constexpr bool isNormalized(ElementType et) {
	return (et == ElementType::SNormInt2_10_10_10Rev || et == ElementType::UNormInt2_10_10_10Rev);
}


static void bindAttributeImpl(const PositionedAttribute& attr, size32 stride, uint32_t toVAIndex) {
	glEnableVertexAttribArray(toVAIndex);
	
	auto field = getField(attr);
	auto offsetPtr = reinterpret_cast<void*>(attr.offset);

	switch (field.type) {
		case ElementType::Float:
			glVertexAttribPointer(toVAIndex, field.count, GL_FLOAT, GL_FALSE, stride, offsetPtr);
			break;
		case ElementType::Double:
			glVertexAttribLPointer(toVAIndex, field.count, GL_DOUBLE, stride, offsetPtr);
			break;
		case ElementType::SInt2_10_10_10Rev:
		case ElementType::UInt2_10_10_10Rev:
		case ElementType::SNormInt2_10_10_10Rev:
		case ElementType::UNormInt2_10_10_10Rev:
			glVertexAttribPointer(toVAIndex, field.count, glTypeForElementType(field.type), isNormalized(field.type), stride, offsetPtr);
			break;
		default:
			glVertexAttribIPointer(toVAIndex, field.count, glTypeForElementType(field.type), stride, offsetPtr);
			break;
	}
}


void GLVertexArray::bindVertexBufferAttributes(const VertexBuffer& vb, uint32_t startBoundIndex) {
	size32 attrCount = vb.attributeCount(),
	         stride = vb.itemSizeBytes();
	
	for (size32 attrIndex = 0; attrIndex < attrCount; ++attrIndex) {
		auto attr = vb.attrByIndex(attrIndex);
		bindAttributeImpl(*attr, stride, attrIndex + startBoundIndex);
	}
}



} // ns render
} // ns stardazed
