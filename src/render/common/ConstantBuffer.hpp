// ------------------------------------------------------------------
// render::ConstantBuffer - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_CONSTANTBUFFER_H
#define SD_RENDER_CONSTANTBUFFER_H

#include "system/Config.hpp"

namespace stardazed {
namespace render {


enum class AccessLevel {
	ReadOnly,
	WriteOnly,
	ReadWrite
};


enum class BufferUpdateFrequency {
	Never,
	Occassionally,
	Frequently
};


} // ns render
} // ns stardazed

#endif
