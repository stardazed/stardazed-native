// ------------------------------------------------------------------
// render::BufferFields - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_BUFFERFIELDS_H
#define SD_RENDER_BUFFERFIELDS_H

#include "system/Config.hpp"

#include <string>
#include <vector>

namespace stardazed {
namespace render {


enum class ElementType {
	UInt8,  SInt8,
	UInt16, SInt16,
	UInt32, SInt32,
	UInt64, SInt64,
	
	Float, Double
};
	

template <ElementType>
struct ElementTraits {};

template <>
struct ElementTraits<ElementType::UInt8>  { using Type = uint8_t; };
template <>
struct ElementTraits<ElementType::SInt8>  { using Type = int8_t; };
template <>
struct ElementTraits<ElementType::UInt16> { using Type = uint16_t; };
template <>
struct ElementTraits<ElementType::SInt16> { using Type = int16_t; };
template <>
struct ElementTraits<ElementType::UInt32> { using Type = uint32_t; };
template <>
struct ElementTraits<ElementType::SInt32> { using Type = int32_t; };
template <>
struct ElementTraits<ElementType::UInt64> { using Type = uint64_t; };
template <>
struct ElementTraits<ElementType::SInt64> { using Type = int64_t; };
template <>
struct ElementTraits<ElementType::Float>  { using Type = float; };
template <>
struct ElementTraits<ElementType::Double> { using Type = double; };

template <ElementType ET>
using ElementNativeType = typename ElementTraits<ET>::Type;


constexpr size32_t elementSize(ElementType et) {
	switch (et) {
		case ElementType::UInt8:  return sizeof(ElementNativeType<ElementType::UInt8>);
		case ElementType::SInt8:  return sizeof(ElementNativeType<ElementType::SInt8>);
		case ElementType::UInt16: return sizeof(ElementNativeType<ElementType::UInt16>);
		case ElementType::SInt16: return sizeof(ElementNativeType<ElementType::SInt16>);
		case ElementType::UInt32: return sizeof(ElementNativeType<ElementType::UInt32>);
		case ElementType::SInt32: return sizeof(ElementNativeType<ElementType::SInt32>);
		case ElementType::UInt64: return sizeof(ElementNativeType<ElementType::UInt64>);
		case ElementType::SInt64: return sizeof(ElementNativeType<ElementType::SInt64>);

		case ElementType::Float:  return sizeof(ElementNativeType<ElementType::Float>);
		case ElementType::Double: return sizeof(ElementNativeType<ElementType::Double>);
	}
}


struct Field {
	ElementType type;
	size32_t count;
};


struct NamedField {
	Field field;
	std::string name;
};



} // ns render
} // ns stardazed

#endif
