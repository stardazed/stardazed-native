// ------------------------------------------------------------------
// render::Buffer - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_BUFFER_H
#define SD_RENDER_BUFFER_H

#include "system/Config.hpp"
#include "math/Vector.hpp"

#include <string>
#include <vector>

namespace stardazed {
namespace render {


enum class AttributeFormat : uint8_t {
	UByte,  SByte,
	UShort, SShort,
	UInt,   SInt,
	ULong,  SLong,
	
	Float,
	Float2,
	Float3,
	Float4,
	Float16,
	
	Double,
	Double2,
	Double3,
	Double4,
	Double16
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

template <>
struct FormatTraits<AttributeFormat::Float> {
	using BaseType = float;
	using ContainerType = float;
	constexpr static const size32_t Size = 4;
	constexpr static const size32_t Align = 4;
};

template <>
struct FormatTraits<AttributeFormat::Float2> {
	using BaseType = float;
	using ContainerType = math::Vector<2, float>;
	constexpr static const size32_t Size = 8;
	constexpr static const size32_t Align = 8;
};

template <>
struct FormatTraits<AttributeFormat::Float3> {
	using BaseType = float;
	using ContainerType = math::Vector<3, float>;
	constexpr static const size32_t Size = 12;
	constexpr static const size32_t Align = 16;
};

template <>
struct FormatTraits<AttributeFormat::Float4> {
	using BaseType = float;
	using ContainerType = math::Vector<4, float>;
	constexpr static const size32_t Size = 16;
	constexpr static const size32_t Align = 16;
};

template <>
struct FormatTraits<AttributeFormat::Float16> {
	using BaseType = float;
	using ContainerType = math::Vector<16, float>;
	constexpr static const size32_t Size = 64;
	constexpr static const size32_t Align = 16;
};

template <>
struct FormatTraits<AttributeFormat::Double> {
	using BaseType = double;
	using ContainerType = double;
	constexpr static const size32_t Size = 8;
	constexpr static const size32_t Align = 8;
};

template <>
struct FormatTraits<AttributeFormat::Double2> {
	using BaseType = double;
	using ContainerType = math::Vector<2, double>;
	constexpr static const size32_t Size = 16;
	constexpr static const size32_t Align = 16;
};

template <>
struct FormatTraits<AttributeFormat::Double3> {
	using BaseType = double;
	using ContainerType = math::Vector<3, double>;
	constexpr static const size32_t Size = 24;
	constexpr static const size32_t Align = 32;
};

template <>
struct FormatTraits<AttributeFormat::Double4> {
	using BaseType = double;
	using ContainerType = math::Vector<4, double>;
	constexpr static const size32_t Size = 32;
	constexpr static const size32_t Align = 32;
};

template <>
struct FormatTraits<AttributeFormat::Double16> {
	using BaseType = double;
	using ContainerType = math::Vector<16, double>;
	constexpr static const size32_t Size = 128;
	constexpr static const size32_t Align = 32;
};


enum class AttributeRole {
	Generic,
	Position,
	Normal,
	Tangent,
	Colour,
	UV, UVW
};


struct AttrBufferComponent {
	std::string name;
	AttributeFormat format;
	AttributeRole role;
};


using AttrBufferFormat = std::vector<AttrBufferComponent>;


class AttributeBuffer {
	AttrBufferFormat format_;

public:
	AttributeBuffer(AttrBufferFormat format, size32_t itemCount);
	
};




} // ns render
} // ns stardazed

#endif
