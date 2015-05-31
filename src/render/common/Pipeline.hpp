// ------------------------------------------------------------------
// render::Pipeline - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_PIPELINE_H
#define SD_RENDER_PIPELINE_H

#include "system/Config.hpp"
#include "render/common/VertexLayout.hpp"
#include "render/common/PixelBuffer.hpp"
#include "render/common/FrameBuffer.hpp"

#include <array>

namespace stardazed {
namespace render {


enum class BlendOperation {
	Add,
	Subtract,
	ReverseSubtract,
	Min,
	Max
};


enum class BlendFactor {
	Zero,
	One,
	SourceColour,
	OneMinusSourceColour,
	DestColour,
	OneMinusDestColour,
	SourceAlpha,
	OneMinusSourceAlpha,
	SourceAlphaSaturated,
	DestAlpha,
	OneMinusDestAlpha,
	ConstantColour,
	OneMinusConstantColour,
	ConstantAlpha,
	OneMinusConstantAlpha
};


struct ColourBlendingDescriptor {
	bool32 enabled = false;

	BlendOperation rgbBlendOp = BlendOperation::Add;
	BlendOperation alphaBlendOp = BlendOperation::Add;

	BlendFactor sourceRGBFactor = BlendFactor::One;
	BlendFactor sourceAlphaFactor = BlendFactor::One;
	BlendFactor destRGBFactor = BlendFactor::Zero;
	BlendFactor destAlphaFactor = BlendFactor::Zero;
};


struct ColourWriteMask {
	bool8 red = true, green = true, blue = true, alpha = true;
};


struct PipelineColourAttachmentDescriptor {
	PixelFormat pixelFormat = PixelFormat::None;
	ColourWriteMask writeMask {};
	ColourBlendingDescriptor blending {};
};


class Shader;


struct PipelineDescriptor {
	std::array<PipelineColourAttachmentDescriptor, maxColourAttachments()> colourAttachments {};
	PixelFormat depthPixelFormat = PixelFormat::None;
	PixelFormat stencilPixelFormat = PixelFormat::None;

	VertexAttributeList vertexAttributes {};
	Shader* vertexShader = nullptr;
	Shader* geometryShader = nullptr;
	Shader* fragmentShader = nullptr;
	
	bool32 rasterizerEnabled = true;
};


} // ns render
} // ns stardazed

#endif
