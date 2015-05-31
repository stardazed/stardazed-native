// ------------------------------------------------------------------
// render::VertexLayout - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_VERTEXLAYOUT_H
#define SD_RENDER_VERTEXLAYOUT_H

#include "system/Config.hpp"

#include <array>
#include <vector>
#include <functional>

namespace stardazed {
namespace render {


// -- A single field in a vertex buffer
// -- 3 properties: element type, count and normalization

enum class VertexField : uint8 {
	Undefined,

	// integer
	UInt8x2,
	UInt8x3,
	UInt8x4,

	SInt8x2,
	SInt8x3,
	SInt8x4,

	UInt16x2,
	UInt16x3,
	UInt16x4,

	SInt16x2,
	SInt16x3,
	SInt16x4,

	UInt32,
	UInt32x2,
	UInt32x3,
	UInt32x4,
	
	SInt32,
	SInt32x2,
	SInt32x3,
	SInt32x4,
	
	// floating point
	Halfx2,
	Halfx3,
	Halfx4,

	Float,
	Floatx2,
	Floatx3,
	Floatx4,

	// normalized
	Norm_UInt8x2 = 0x81,	// normalized fields have high bit set
	Norm_UInt8x3,
	Norm_UInt8x4,
	
	Norm_SInt8x2,
	Norm_SInt8x3,
	Norm_SInt8x4,
	
	Norm_UInt16x2,
	Norm_UInt16x3,
	Norm_UInt16x4,
	
	Norm_SInt16x2,
	Norm_SInt16x3,
	Norm_SInt16x4,
	
	// normalized packed types
	Norm_SInt2_10_10_10Rev,
	Norm_UInt2_10_10_10Rev
};


// --- VertexField traits

constexpr size32 vertexFieldElementCount(VertexField vf) {
	switch (vf) {
		case VertexField::Undefined:
			return 0;
		
		case VertexField::UInt32:
		case VertexField::SInt32:
		case VertexField::Float:
		case VertexField::Norm_SInt2_10_10_10Rev:
		case VertexField::Norm_UInt2_10_10_10Rev:
			return 1;
		
		case VertexField::UInt8x2:
		case VertexField::Norm_UInt8x2:
		case VertexField::SInt8x2:
		case VertexField::Norm_SInt8x2:
		case VertexField::UInt16x2:
		case VertexField::Norm_UInt16x2:
		case VertexField::SInt16x2:
		case VertexField::Norm_SInt16x2:
		case VertexField::UInt32x2:
		case VertexField::SInt32x2:
		case VertexField::Halfx2:
		case VertexField::Floatx2:
			return 2;
		
		case VertexField::UInt8x3:
		case VertexField::Norm_UInt8x3:
		case VertexField::SInt8x3:
		case VertexField::Norm_SInt8x3:
		case VertexField::UInt16x3:
		case VertexField::Norm_UInt16x3:
		case VertexField::SInt16x3:
		case VertexField::Norm_SInt16x3:
		case VertexField::UInt32x3:
		case VertexField::SInt32x3:
		case VertexField::Halfx3:
		case VertexField::Floatx3:
			return 3;
			
		case VertexField::UInt8x4:
		case VertexField::Norm_UInt8x4:
		case VertexField::SInt8x4:
		case VertexField::Norm_SInt8x4:
		case VertexField::UInt16x4:
		case VertexField::Norm_UInt16x4:
		case VertexField::SInt16x4:
		case VertexField::Norm_SInt16x4:
		case VertexField::UInt32x4:
		case VertexField::SInt32x4:
		case VertexField::Halfx4:
		case VertexField::Floatx4:
			return 4;
	}
}


constexpr size32 vertexFieldElementSizeBytes(VertexField vf) {
	switch (vf) {
		case VertexField::Undefined:
			return 0;
			
		case VertexField::Float:
		case VertexField::Floatx2:
		case VertexField::Floatx3:
		case VertexField::Floatx4:
		case VertexField::UInt32:
		case VertexField::SInt32:
		case VertexField::UInt32x2:
		case VertexField::SInt32x2:
		case VertexField::UInt32x3:
		case VertexField::SInt32x3:
		case VertexField::UInt32x4:
		case VertexField::SInt32x4:
		case VertexField::Norm_SInt2_10_10_10Rev:
		case VertexField::Norm_UInt2_10_10_10Rev:
			return 4;
			
		case VertexField::UInt16x2:
		case VertexField::Norm_UInt16x2:
		case VertexField::SInt16x2:
		case VertexField::Norm_SInt16x2:
		case VertexField::UInt16x3:
		case VertexField::Norm_UInt16x3:
		case VertexField::SInt16x3:
		case VertexField::Norm_SInt16x3:
		case VertexField::UInt16x4:
		case VertexField::Norm_UInt16x4:
		case VertexField::SInt16x4:
		case VertexField::Norm_SInt16x4:
		case VertexField::Halfx2:
		case VertexField::Halfx3:
		case VertexField::Halfx4:
			return 2;
			
		case VertexField::UInt8x2:
		case VertexField::Norm_UInt8x2:
		case VertexField::SInt8x2:
		case VertexField::Norm_SInt8x2:
		case VertexField::UInt8x3:
		case VertexField::Norm_UInt8x3:
		case VertexField::SInt8x3:
		case VertexField::Norm_SInt8x3:
		case VertexField::UInt8x4:
		case VertexField::Norm_UInt8x4:
		case VertexField::SInt8x4:
		case VertexField::Norm_SInt8x4:
			return 1;
	}
}


constexpr size32 vertexFieldSizeBytes(VertexField vf) {
	return vertexFieldElementSizeBytes(vf) * vertexFieldElementCount(vf);
}


constexpr bool vertexFieldIsNormalized(VertexField vf) {
	auto raw = static_cast<std::underlying_type_t<VertexField>>(vf);
	return (raw & 0x80) != 0;
}


enum class VertexAttributeRole : uint8 {
	Generic,
	Position,
	Normal,
	Tangent,
	Colour,
	UV,
	UVW,
	Index
};


// -- An VertexAttribute is a Field with a certain Role inside a VertexBuffer

struct VertexAttribute {
	VertexField field = VertexField::Undefined;
	VertexAttributeRole role = VertexAttributeRole::Generic;
};


constexpr size32 maxVertexAttributes() {
	// FIXME - this is the mandated minimum for GL 4.4
	// may want to up this to 32 and limit actual usage based on
	// runtime reported maximum (GL_MAX_VERTEX_ATTRIBS)
	return 16;
}


// -- VertexAttribute shortcuts for common types

constexpr VertexAttribute attrPosition3() { return { VertexField::Floatx3, VertexAttributeRole::Position }; }
constexpr VertexAttribute attrNormal3()   { return { VertexField::Floatx3, VertexAttributeRole::Normal }; }
constexpr VertexAttribute attrColour3()   { return { VertexField::Floatx3, VertexAttributeRole::Colour }; }
constexpr VertexAttribute attrUV2()       { return { VertexField::Floatx2, VertexAttributeRole::UV }; }
constexpr VertexAttribute attrTangent4()  { return { VertexField::Floatx4, VertexAttributeRole::Tangent }; }


// -- A VertexAttributeList defines the structure of a VertexBuffer

using VertexAttributeList = std::vector<VertexAttribute>;


// -- Common AttributeList shortcuts

namespace AttrList {
	inline VertexAttributeList Pos3Norm3() {
		return { attrPosition3(), attrNormal3() };
	}
	inline VertexAttributeList Pos3Norm3UV2() {
		return { attrPosition3(), attrNormal3(), attrUV2() };
	}
	inline VertexAttributeList Pos3Norm3UV2Tan4() {
		return { attrPosition3(), attrNormal3(), attrUV2(), attrTangent4() };
	}
}


struct PositionedAttribute : VertexAttribute {
	size16 offset = 0;

	PositionedAttribute() = default;

	constexpr PositionedAttribute(VertexField vf, VertexAttributeRole ar, size32 offset)
	: VertexAttribute{ vf, ar}
	, offset { static_cast<size16>(offset) }
	{}

	constexpr PositionedAttribute(VertexAttribute attr, size32 offset)
	: VertexAttribute(attr)
	, offset { static_cast<size16>(offset) }
	{}
};


class VertexLayout {
	size32 attributeCount_ = 0, vertexSizeBytes_ = 0;
	std::array<PositionedAttribute, maxVertexAttributes()> attrs_;

	const PositionedAttribute* attrByPredicate(std::function<bool(const PositionedAttribute&)>) const;

public:
	VertexLayout(const VertexAttributeList&);
	
	size32 attributeCount() const { return attributeCount_; }
	size32 vertexSizeBytes() const { return vertexSizeBytes_; }
	
	size32 bytesRequiredForVertexCount(size32 vertexCount) const {
		return vertexCount * vertexSizeBytes();
	}
	
	const PositionedAttribute* attrByRole(VertexAttributeRole) const;
	const PositionedAttribute* attrByIndex(size32) const;
	
	bool hasAttributeWithRole(VertexAttributeRole role) const {
		return attrByRole(role) != nullptr;
	}
};


} // ns render
} // ns stardazed

#endif
