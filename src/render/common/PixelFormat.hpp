// ------------------------------------------------------------------
// render::PixelFormat - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_PIXELFORMAT_H
#define SD_RENDER_PIXELFORMAT_H

#include "system/Config.hpp"

#include <vector>

namespace stardazed {
namespace render {


enum class PixelFormat {
	None,

	// basic formats
	RGBA_8888,
	ABGR_8888
};


enum class ComponentFormat : uint8_t {
	UByte,  SByte,
	UShort, SShort,
	UInt,   SInt,
	ULong,  SLong,

	Float
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
constexpr const size32_t FormatSize = FormatTraits<CF>::Size;

template <ComponentFormat CF>
constexpr const size32_t FormatAlign = FormatTraits<CF>::Align;


template <>
struct FormatTraits<ComponentFormat::UByte> {
	using BaseType = uint8_t;
	constexpr const size32_t Size = 1;
	constexpr const size32_t Align = 1;
};

template <>
struct FormatTraits<ComponentFormat::SByte> {
	using BaseType = int8_t;
	constexpr const size32_t Size = 1;
	constexpr const size32_t Align = 1;
};

template <>
struct FormatTraits<ComponentFormat::UShort> {
	using BaseType = uint16_t;
	constexpr const size32_t Size = 2;
	constexpr const size32_t Align = 2;
};

template <>
struct FormatTraits<ComponentFormat::SShort> {
	using BaseType = int16_t;
	constexpr const size32_t Size = 2;
	constexpr const size32_t Align = 2;
};

template <>
struct FormatTraits<ComponentFormat::UInt> {
	using BaseType = uint32_t;
	constexpr const size32_t Size = 4;
	constexpr const size32_t Align = 4;
};

template <>
struct FormatTraits<ComponentFormat::SInt> {
	using BaseType = int32_t;
	constexpr const size32_t Size = 4;
	constexpr const size32_t Align = 4;
};

template <>
struct FormatTraits<ComponentFormat::ULong> {
	using BaseType = uint64_t;
	constexpr const size32_t Size = 8;
	constexpr const size32_t Align = 8;
};

template <>
struct FormatTraits<ComponentFormat::SLong> {
	using BaseType = int64_t;
	constexpr const size32_t Size = 8;
	constexpr const size32_t Align = 8;
};

template <>
struct FormatTraits<ComponentFormat::Float> {
	using BaseType = float;
	constexpr const size32_t Size = 4;
	constexpr const size32_t Align = 4;
};

template <>
struct FormatTraits<ComponentFormat::Float2> {
	using BaseType = float;
	constexpr const size32_t Size = 8;
	constexpr const size32_t Align = 8;
};

template <>
struct FormatTraits<ComponentFormat::Float3> {
	using BaseType = float;
	constexpr const size32_t Size = 12;
	constexpr const size32_t Align = 16;
};

template <>
struct FormatTraits<ComponentFormat::Float4> {
	using BaseType = float;
	constexpr const size32_t Size = 16;
	constexpr const size32_t Align = 16;
};

template <>
struct FormatTraits<ComponentFormat::Float16> {
	using BaseType = float;
	constexpr const size32_t Size = 64;
	constexpr const size32_t Align = 16;
};

template <>
struct FormatTraits<ComponentFormat::Double> {
	using BaseType = double;
	constexpr const size32_t Size = 8;
	constexpr const size32_t Align = 8;
};

template <>
struct FormatTraits<ComponentFormat::Double2> {
	using BaseType = double;
	constexpr const size32_t Size = 16;
	constexpr const size32_t Align = 16;
};

template <>
struct FormatTraits<ComponentFormat::Double3> {
	using BaseType = double;
	constexpr const size32_t Size = 24;
	constexpr const size32_t Align = 32;
};

template <>
struct FormatTraits<ComponentFormat::Double4> {
	using BaseType = double;
	constexpr const size32_t Size = 32;
	constexpr const size32_t Align = 32;
};

template <>
struct FormatTraits<ComponentFormat::Double16> {
	using BaseType = double;
	constexpr const size32_t Size = 128;
	constexpr const size32_t Align = 32;
};



enum class ComponentRole {
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


struct TextureDescriptor {
	PixelFormat format;
};


} // ns render
} // ns stardazed

#endif
