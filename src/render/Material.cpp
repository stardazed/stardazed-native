// ------------------------------------------------------------------
// render::Material - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "render/Material.hpp"

namespace stardazed {
namespace render {


void Material::activate() {
	pipeline_->activate();
}


} // ns render
} // ns stardazed
