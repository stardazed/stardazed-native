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


enum class ComponentFormat : uint8_t {
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


template <ComponentFormat CF>
struct FormatTraits {};

template <ComponentFormat CF>
using FormatNativeType = typename FormatTraits<CF>::BaseType;

template <ComponentFormat CF>
using FormatContainerType = typename FormatTraits<CF>::ContainerType;
	
template <ComponentFormat CF>
constexpr const size32_t FormatSize = FormatTraits<CF>::Size;

template <ComponentFormat CF>
constexpr const size32_t FormatAlign = FormatTraits<CF>::Align;


template <>
struct FormatTraits<ComponentFormat::UByte> {
	using BaseType = uint8_t;
	using ContainerType = uint8_t;
	constexpr static const size32_t Size = 1;
	constexpr static const size32_t Align = 1;
};

template <>
struct FormatTraits<ComponentFormat::SByte> {
	using BaseType = int8_t;
	using ContainerType = int8_t;
	constexpr static const size32_t Size = 1;
	constexpr static const size32_t Align = 1;
};

template <>
struct FormatTraits<ComponentFormat::UShort> {
	using BaseType = uint16_t;
	using ContainerType = uint16_t;
	constexpr static const size32_t Size = 2;
	constexpr static const size32_t Align = 2;
};

template <>
struct FormatTraits<ComponentFormat::SShort> {
	using BaseType = int16_t;
	using ContainerType = int16_t;
	constexpr static const size32_t Size = 2;
	constexpr static const size32_t Align = 2;
};

template <>
struct FormatTraits<ComponentFormat::UInt> {
	using BaseType = uint32_t;
	using ContainerType = uint32_t;
	constexpr static const size32_t Size = 4;
	constexpr static const size32_t Align = 4;
};

template <>
struct FormatTraits<ComponentFormat::SInt> {
	using BaseType = int32_t;
	using ContainerType = int32_t;
	constexpr static const size32_t Size = 4;
	constexpr static const size32_t Align = 4;
};

template <>
struct FormatTraits<ComponentFormat::ULong> {
	using BaseType = uint64_t;
	using ContainerType = uint64_t;
	constexpr static const size32_t Size = 8;
	constexpr static const size32_t Align = 8;
};

template <>
struct FormatTraits<ComponentFormat::SLong> {
	using BaseType = int64_t;
	using ContainerType = int64_t;
	constexpr static const size32_t Size = 8;
	constexpr static const size32_t Align = 8;
};

template <>
struct FormatTraits<ComponentFormat::Float> {
	using BaseType = float;
	using ContainerType = float;
	constexpr static const size32_t Size = 4;
	constexpr static const size32_t Align = 4;
};

template <>
struct FormatTraits<ComponentFormat::Float2> {
	using BaseType = float;
	using ContainerType = math::Vector<2, float>;
	constexpr static const size32_t Size = 8;
	constexpr static const size32_t Align = 8;
};

template <>
struct FormatTraits<ComponentFormat::Float3> {
	using BaseType = float;
	using ContainerType = math::Vector<3, float>;
	constexpr static const size32_t Size = 12;
	constexpr static const size32_t Align = 16;
};

template <>
struct FormatTraits<ComponentFormat::Float4> {
	using BaseType = float;
	using ContainerType = math::Vector<4, float>;
	constexpr static const size32_t Size = 16;
	constexpr static const size32_t Align = 16;
};

template <>
struct FormatTraits<ComponentFormat::Float16> {
	using BaseType = float;
	using ContainerType = math::Vector<16, float>;
	constexpr static const size32_t Size = 64;
	constexpr static const size32_t Align = 16;
};

template <>
struct FormatTraits<ComponentFormat::Double> {
	using BaseType = double;
	using ContainerType = double;
	constexpr static const size32_t Size = 8;
	constexpr static const size32_t Align = 8;
};

template <>
struct FormatTraits<ComponentFormat::Double2> {
	using BaseType = double;
	using ContainerType = math::Vector<2, double>;
	constexpr static const size32_t Size = 16;
	constexpr static const size32_t Align = 16;
};

template <>
struct FormatTraits<ComponentFormat::Double3> {
	using BaseType = double;
	using ContainerType = math::Vector<3, double>;
	constexpr static const size32_t Size = 24;
	constexpr static const size32_t Align = 32;
};

template <>
struct FormatTraits<ComponentFormat::Double4> {
	using BaseType = double;
	using ContainerType = math::Vector<4, double>;
	constexpr static const size32_t Size = 32;
	constexpr static const size32_t Align = 32;
};

template <>
struct FormatTraits<ComponentFormat::Double16> {
	using BaseType = double;
	using ContainerType = math::Vector<16, double>;
	constexpr static const size32_t Size = 128;
	constexpr static const size32_t Align = 32;
};


enum class ComponentRole {
	Generic,
	Position,
	Normal,
	Tangent,
	Colour,
	UV, UVW
};


struct BufferComponent {
	std::string name;
	ComponentFormat format;
	ComponentRole role;
	std::vector<char> data;
};


using BufferFormat = std::vector<BufferComponent>;





} // ns render
} // ns stardazed

#endif
