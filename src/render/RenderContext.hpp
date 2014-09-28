// ------------------------------------------------------------------
// render::RenderContext forwarder - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "system/Config.hpp"

#if SD_PLATFORM_OSX
#	if SD_RENDER_ENGINE_OPENGL
#		include "render/opengl/mac_RenderContext.hpp"
#	else
#		error "Unsupported Render Engine."
#	endif
#else
#	error "Platform/Render Engine combo is not supported."
#endif
