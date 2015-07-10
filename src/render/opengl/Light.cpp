// ------------------------------------------------------------------
// render::opengl::Light.cpp - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#include "render/opengl/Light.hpp"

namespace stardazed {
namespace render {


Light::Light()
: shaderLights_{ BufferRole::ConstantBuffer, BufferUpdateFrequency::Frequently, BufferClientAccess::WriteOnly }
{
	maxShaderLights_ = ConstantBufferLimits::maximumAccessibleArrayElementsPerBlock<LightDescriptor>();
	shaderBlockSizeBytes_ = maxShaderLights_ * sizeof32<LightDescriptor>();
	
	auto uniformOffsetAlignment = ConstantBufferLimits::offsetAlignment();
	shaderLights_.allocate(math::alignUp(shaderBlockSizeBytes_, uniformOffsetAlignment));
}


} // ns render
} // ns stardazed
