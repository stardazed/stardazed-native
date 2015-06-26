// ------------------------------------------------------------------
// render::Technique - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_TECHNIQUE_H
#define SD_RENDER_TECHNIQUE_H

#include "system/Config.hpp"
#include "math/Matrix.hpp"
#include "render/common/RenderPass.hpp"

namespace stardazed {
namespace render {


struct ProjectionSetup {
	math::Mat4 projMat, viewMat, viewProjMat;
	
	ProjectionSetup(const math::Mat4& projectionMatrix, const math::Mat4& viewMatrix)
	: projMat { projectionMatrix }
	, viewMat { viewMatrix }
	, viewProjMat { projMat * viewMat }
	{}
};


struct Technique {
	virtual ~Technique() = default;
	
	virtual void start(RenderPass&, const ProjectionSetup&) = 0;
	virtual void stop() {}
};


} // ns render
} // ns stardazed

#endif
