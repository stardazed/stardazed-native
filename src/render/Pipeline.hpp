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


template <typename Shader>
class Pipeline {
public:
	virtual ~Pipeline() = default;
	virtual void attachShader(Shader*) = 0;
	
	virtual void activate() = 0;
	virtual void deactivate() = 0;
};


} // ns render
} // ns stardazed

#endif
