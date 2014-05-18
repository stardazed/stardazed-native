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
	
	virtual void setVertexStage(const VertexShaderRef&) = 0;
	virtual void setTesselationControlStage(const TesselationControlShaderRef&) = 0;
	virtual void setTesselationEvalStage(const TesselationEvalShaderRef&) = 0;
	virtual void setGeometryStage(const GeometryShaderRef&) = 0;
	virtual void setFragmentStage(const FragmentShaderRef&) = 0;
	
	virtual void activate() = 0;
	virtual void deactivate() = 0;
};


using PipelineRef = std::shared_ptr<Pipeline>;


} // ns render
} // ns stardazed

#endif
