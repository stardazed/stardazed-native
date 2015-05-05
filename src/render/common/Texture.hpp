// ------------------------------------------------------------------
// render::Texture - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_TEXTURE_H
#define SD_RENDER_TEXTURE_H

#include "system/Config.hpp"

namespace stardazed {
namespace render {


enum class CubeMapFace {
	PosX,
	NegX,
	PosY,
	NegY,
	PosZ,
	NegZ
};


} // ns render
} // ns stardazed

#endif
