// ------------------------------------------------------------------
// render::Shader - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_SHADER_H
#define SD_RENDER_SHADER_H

#include <memory>

namespace stardazed {
namespace render {


enum class ShaderType {
	Vertex,
	TesselationControl, // DirectX term: hull
	TesselationEval,    // DirectX term: domain
	Geometry,
	Fragment,           // DirectX term: pixel
	Compute
};


template <ShaderType Type>
class Shader {
public:
	virtual ~Shader() = default;
};


template <ShaderType Type>
using ShaderRef                = std::shared_ptr<Shader<Type>>;

using VertexShader             = Shader<ShaderType::Vertex>;
using TesselationControlShader = Shader<ShaderType::TesselationControl>;
using TesselationEvalShader    = Shader<ShaderType::TesselationEval>;
using GeometryShader           = Shader<ShaderType::Geometry>;
using FragmentShader           = Shader<ShaderType::Fragment>;
using ComputeShader            = Shader<ShaderType::Compute>;


using VertexShaderRef             = std::shared_ptr<VertexShader>;
using TesselationControlShaderRef = std::shared_ptr<TesselationControlShader>;
using TesselationEvalShaderRef    = std::shared_ptr<TesselationEvalShader>;
using GeometryShaderRef           = std::shared_ptr<GeometryShader>;
using FragmentShaderRef           = std::shared_ptr<FragmentShader>;
using ComputeShaderRef            = std::shared_ptr<ComputeShader>;


} // ns render
} // ns stardazed

#endif
