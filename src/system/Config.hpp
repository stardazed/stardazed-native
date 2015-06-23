// ------------------------------------------------------------------
// system::Config - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_SYSTEM_CONFIG_H
#define SD_SYSTEM_CONFIG_H


// -- platform detection and setup

#ifdef _WIN64
#	define SD_PLATFORM_WINDOWS 1
#	define SD_PLATFORM_OSX     0
#elif _WIN32
#	error "32-bit Windows is not supported"
#elif __APPLE__
#	include "TargetConditionals.h"
#	if TARGET_OS_IPHONE && TARGET_IPHONE_SIMULATOR
#		error "iOS is not supported"
#	elif TARGET_OS_IPHONE
#		error "iOS is not supported"
#	else
#		define SD_PLATFORM_WINDOWS 0
#		define SD_PLATFORM_OSX     1
#	endif
#else
#	error "This platform is not supported"
#endif


// -- render engine

#define SD_RENDER_ENGINE_OPENGL 1


// -- compiler features

#if __has_feature(nullability) || (__apple_build_version__ >= 6020053)
#	define NON_NULL __nonnull
#	define NULLABLE __nullable
#else
#	define NON_NULL
#	define NULLABLE
#endif


// -- stardazed namespace and alias

namespace stardazed {}
namespace sd = stardazed;


// -- common headers

#include <cstdint>
#include <cassert>


namespace stardazed {


// -- common types
using uint   = unsigned int;

using int8   = int8_t;
using uint8  = uint8_t;
using int16  = int16_t;
using uint16 = uint16_t;
using int32  = int32_t;
using uint32 = uint32_t;
using int64  = int64_t;
using uint64 = uint64_t;

using size16 = uint16_t;
using size32 = uint32_t; // 64-bit sizes are often overkill, use size32 for those cases
using size64 = uint64_t;

using bool8  = uint8_t;
using bool16 = uint16_t; // wide boolean types
using bool32 = uint32_t;


// -- helpers for size32 usage with STL
template <typename Value>
constexpr size32 size32_cast(const Value v) {
	return static_cast<size32>(v);
}

template <typename T>
constexpr size32 sizeof32() {
	return size32_cast(sizeof(T));
}


} // ns stardazed


#endif
