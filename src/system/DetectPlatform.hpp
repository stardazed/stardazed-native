// ------------------------------------------------------------------
// system::DetectPlatform - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_SYSTEM_DETECTPLATFORM_H
#define SD_SYSTEM_DETECTPLATFORM_H


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


#endif
