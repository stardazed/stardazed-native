// ------------------------------------------------------------------
// render::Pipeline - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_PIPELINE_H
#define SD_RENDER_PIPELINE_H

#include "render/Shader.hpp"

#include <memory>

namespace stardazed {
namespace render {


class Pipeline {
public:
	virtual ~Pipeline() = default;
	
	virtual void setVertexStage(VertexShader) = 0;
	virtual void setTesselationControlStage(TesselationControlShader) = 0;
	virtual void setTesselationEvalStage(TesselationEvalShader) = 0;
	virtual void setGeometryStage(GeometryShader) = 0;
	virtual void setFragmentStage(FragmentShader) = 0;
	
	virtual void activate() = 0;
	virtual void deactivate() = 0;
};


} // ns render
} // ns stardazed

#endif
