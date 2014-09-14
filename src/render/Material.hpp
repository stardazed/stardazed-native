// ------------------------------------------------------------------
// render::Material - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_MATERIAL_H
#define SD_RENDER_MATERIAL_H

#include "system/Config.hpp"
#include "render/Pipeline.hpp"

namespace stardazed {
namespace render {


// NOT IN USE YET


class Material {
	Pipeline* pipeline_;

public:
	Material(Pipeline* pipeline) : pipeline_(pipeline) {}
	virtual ~Material() {}

	virtual void activate();
};


} // ns render
} // ns stardazed

#endif
