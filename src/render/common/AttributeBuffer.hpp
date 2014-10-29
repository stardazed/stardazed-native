// ------------------------------------------------------------------
// render::AttributeBuffer - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_ATTRIBUTEBUFFER_H
#define SD_RENDER_ATTRIBUTEBUFFER_H

#include "system/Config.hpp"
#include "math/Vector.hpp"
#include "math/Matrix.hpp"

#include <string>
#include <vector>
#include <memory>

namespace stardazed {
namespace render {


enum class ComponentAlignment {
	Natural,
	GLStd140
};


enum class AttributeFormat : uint16_t {
	UByte,  SByte,
	UShort, SShort,
	UInt,   SInt,
	ULong,  SLong,
	
	Float,
	Vec2,
	Vec3,
	Vec4,
	Mat3,
	Mat4,
	
	Double,
	DVec2,
	DVec3,
	DVec4,
	DMat3,
	DMat4
};


template <AttributeFormat CF>
struct FormatTraits {};

template <AttributeFormat CF>
using FormatNativeType = typename FormatTraits<CF>::BaseType;

template <AttributeFormat CF>
using FormatContainerType = typename FormatTraits<CF>::ContainerType;
	
template <AttributeFormat CF>
constexpr const size32_t FormatSize = FormatTraits<CF>::Size;

template <AttributeFormat CF>
constexpr const size32_t FormatAlign = FormatTraits<CF>::Align;

template <AttributeFormat CF>
constexpr const size32_t FormatMinAlign = sizeof(typename FormatTraits<CF>::BaseType);


// -- Integer Fields

template <>
struct FormatTraits<AttributeFormat::UByte> {
	using BaseType = uint8_t;
	using ContainerType = uint8_t;
	constexpr static const size32_t Size = 1;
	constexpr static const size32_t Align = 1;
};

template <>
struct FormatTraits<AttributeFormat::SByte> {
	using BaseType = int8_t;
	using ContainerType = int8_t;
	constexpr static const size32_t Size = 1;
	constexpr static const size32_t Align = 1;
};

template <>
struct FormatTraits<AttributeFormat::UShort> {
	using BaseType = uint16_t;
	using ContainerType = uint16_t;
	constexpr static const size32_t Size = 2;
	constexpr static const size32_t Align = 2;
};

template <>
struct FormatTraits<AttributeFormat::SShort> {
	using BaseType = int16_t;
	using ContainerType = int16_t;
	constexpr static const size32_t Size = 2;
	constexpr static const size32_t Align = 2;
};

template <>
struct FormatTraits<AttributeFormat::UInt> {
	using BaseType = uint32_t;
	using ContainerType = uint32_t;
	constexpr static const size32_t Size = 4;
	constexpr static const size32_t Align = 4;
};

template <>
struct FormatTraits<AttributeFormat::SInt> {
	using BaseType = int32_t;
	using ContainerType = int32_t;
	constexpr static const size32_t Size = 4;
	constexpr static const size32_t Align = 4;
};

template <>
struct FormatTraits<AttributeFormat::ULong> {
	using BaseType = uint64_t;
	using ContainerType = uint64_t;
	constexpr static const size32_t Size = 8;
	constexpr static const size32_t Align = 8;
};

template <>
struct FormatTraits<AttributeFormat::SLong> {
	using BaseType = int64_t;
	using ContainerType = int64_t;
	constexpr static const size32_t Size = 8;
	constexpr static const size32_t Align = 8;
};


// -- Float-based Fields

template <>
struct FormatTraits<AttributeFormat::Float> {
	using BaseType = float;
	using ContainerType = float;
	constexpr static const size32_t Size = 4;
	constexpr static const size32_t Align = 4;
};

template <>
struct FormatTraits<AttributeFormat::Vec2> {
	using BaseType = float;
	using ContainerType = math::Vector<2, float>;
	constexpr static const size32_t Size = 8;
	constexpr static const size32_t Align = 8;
};

template <>
struct FormatTraits<AttributeFormat::Vec3> {
	using BaseType = float;
	using ContainerType = math::Vector<3, float>;
	constexpr static const size32_t Size = 12;
	constexpr static const size32_t Align = 16;
};

template <>
struct FormatTraits<AttributeFormat::Vec4> {
	using BaseType = float;
	using ContainerType = math::Vector<4, float>;
	constexpr static const size32_t Size = 16;
	constexpr static const size32_t Align = 16;
};

template <>
struct FormatTraits<AttributeFormat::Mat3> {
	using BaseType = float;
	using ContainerType = math::Matrix<3, 3, float>;
	constexpr static const size32_t Size = 64;
	constexpr static const size32_t Align = 16;
};

template <>
struct FormatTraits<AttributeFormat::Mat4> {
	using BaseType = float;
	using ContainerType = math::Matrix<4, 4, float>;
	constexpr static const size32_t Size = 64;
	constexpr static const size32_t Align = 16;
};


// -- Double-based Fields

template <>
struct FormatTraits<AttributeFormat::Double> {
	using BaseType = double;
	using ContainerType = double;
	constexpr static const size32_t Size = 8;
	constexpr static const size32_t Align = 8;
};

template <>
struct FormatTraits<AttributeFormat::DVec2> {
	using BaseType = double;
	using ContainerType = math::Vector<2, double>;
	constexpr static const size32_t Size = 16;
	constexpr static const size32_t Align = 16;
};

template <>
struct FormatTraits<AttributeFormat::DVec3> {
	using BaseType = double;
	using ContainerType = math::Vector<3, double>;
	constexpr static const size32_t Size = 24;
	constexpr static const size32_t Align = 32;
};

template <>
struct FormatTraits<AttributeFormat::DVec4> {
	using BaseType = double;
	using ContainerType = math::Vector<4, double>;
	constexpr static const size32_t Size = 32;
	constexpr static const size32_t Align = 32;
};

template <>
struct FormatTraits<AttributeFormat::DMat3> {
	using BaseType = double;
	using ContainerType = math::Matrix<3, 3, double>;
	constexpr static const size32_t Size = 128;
	constexpr static const size32_t Align = 32;
};

template <>
struct FormatTraits<AttributeFormat::DMat4> {
	using BaseType = double;
	using ContainerType = math::Matrix<4, 4, double>;
	constexpr static const size32_t Size = 128;
	constexpr static const size32_t Align = 32;
};


enum class AttributeRole : uint16_t {
	Generic,
	Position,
	Normal,
	Tangent,
	Colour,
	UV,
	UVW
};


struct AttributeDescriptor {
	std::string name;
	AttributeFormat format;
	AttributeRole role;
};


using AttributeList = std::vector<AttributeDescriptor>;


class AttributeBuffer {
	struct PositionedAttribute {
		AttributeDescriptor attr;
		size32_t offset;
	};

	std::vector<PositionedAttribute> attrList_;
	
	size32_t itemSizeBytes_;
	std::unique_ptr<uint8_t[]> data_;

public:
	AttributeBuffer(const AttributeList&, ComponentAlignment, size32_t itemCount);
	
};




} // ns render
} // ns stardazed

#endif
