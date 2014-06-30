// ------------------------------------------------------------------
// render::Pipeline - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_PIPELINE_H
#define SD_RENDER_PIPELINE_H

#include "render/PixelFormat.hpp"
#include "render/ConstantBuffer.hpp"
#include "render/Shader.hpp"

#include <vector>

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


template <typename Shader>
class Pipeline {
public:
	struct Descriptor {
		FaceCulling faceCulling = FaceCulling::None;
		DepthTestPredicate depthTest = DepthTestPredicate::Disabled;
		
		Shader* vertexShader = nullptr;
		Shader* fragmentShader = nullptr;
		
		std::vector<PixelFormat> colourAttachmentFormats;
		PixelFormat depthAttachmentFormat;
	};


	virtual ~Pipeline() = default;

	virtual ConstantBuffer* constantBuffer() = 0;
	
	virtual void activate() = 0;
	virtual void deactivate() = 0;
};


} // ns render
} // ns stardazed

#endif
