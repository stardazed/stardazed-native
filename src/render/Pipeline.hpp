// ------------------------------------------------------------------
// render::Pipeline - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_PIPELINE_H
#define SD_RENDER_PIPELINE_H

#include "render/PixelFormat.hpp"
#include "render/ConstantBuffer.hpp"
#include "render/Shader.hpp"

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


template <typename ShaderType>
class Pipeline {
public:
	struct Descriptor {
		FaceCulling faceCulling = FaceCulling::Disabled;
		DepthTestPredicate depthTest = DepthTestPredicate::Disabled;
		
		ShaderType* vertexShader = nullptr;
		ShaderType* fragmentShader = nullptr;
	};


	virtual ~Pipeline() = default;

	virtual ConstantBuffer* constantBuffer() = 0;
	
	virtual void activate() = 0;
	virtual void deactivate() = 0;
};


} // ns render
} // ns stardazed

#endif
