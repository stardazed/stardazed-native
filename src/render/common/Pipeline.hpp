// ------------------------------------------------------------------
// render::Pipeline - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_PIPELINE_H
#define SD_RENDER_PIPELINE_H

#include "system/Config.hpp"
#include "render/common/Shader.hpp"

namespace stardazed {
namespace render {


enum class FaceCulling {
	Disabled,

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


class Shader;


struct PipelineDescriptor {
	FaceCulling faceCulling = FaceCulling::Disabled;
	DepthTestPredicate depthTest = DepthTestPredicate::Disabled;
	
	Shader* vertexShader = nullptr;
	Shader* fragmentShader = nullptr;
};


} // ns render
} // ns stardazed

#endif
