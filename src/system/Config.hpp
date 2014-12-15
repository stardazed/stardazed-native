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


// -- stardazed namespace and alias

namespace stardazed {}
namespace sd = stardazed;


// -- common headers

#include <cstdint>
#include <cassert>


namespace stardazed {


// -- common types
using int8   = int8_t;
using uint8  = uint8_t;
using int16  = int16_t;
using uint16 = uint16_t;
using int32  = int32_t;
using uint32 = uint32_t;
using int64  = int64_t;
using uint64 = uint64_t;

using size32 = uint32_t; // 64-bit sizes are often overkill, use size32 for those cases
using size64 = uint64_t;


} // ns stardazed


#endif
