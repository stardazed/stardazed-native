// ------------------------------------------------------------------
// scene::RendererShared - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_SCENE_RENDERERSHARED_H
#define SD_SCENE_RENDERERSHARED_H

#include "system/Config.hpp"
#include "math/Matrix.hpp"
#include "render/common/RenderPass.hpp"

namespace stardazed {
namespace scene {
	
	
struct ProjectionSetup {
	math::Mat4 projMat, viewMat;
	
	ProjectionSetup(const math::Mat4& projectionMatrix, const math::Mat4& viewMatrix)
	: projMat { projectionMatrix }
	, viewMat { viewMatrix }
	{}
};


} // ns scene
} // ns stardazed

#endif
