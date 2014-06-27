// ------------------------------------------------------------------
// render::Pipeline - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_PIPELINE_H
#define SD_RENDER_PIPELINE_H

#include "render/ConstantBuffer.hpp"
#include "render/Shader.hpp"

#include <memory>

namespace stardazed {
namespace render {


enum class FaceCulling {
	None,
	Front,
	Back,
	FrontAndBack
};


enum class DepthTestPredicate {
	Disabled,

	AllowAll,
	DenyAll,
	
	Less,
	LessOrEqual,
	Equal,
	NotEqual,
	GreaterOrEqual,
	Greater
};


struct PipelineDescriptor {
	FaceCulling faceCulling;
	DepthTestPredicate depthTest;
};


template <typename Shader>
class Pipeline {
public:
	virtual ~Pipeline() = default;

	virtual ConstantBuffer* constantBuffer() = 0;
	virtual void attachShader(const Shader&) = 0;
	
	virtual void activate() = 0;
	virtual void deactivate() = 0;
};


} // ns render
} // ns stardazed

#endif
