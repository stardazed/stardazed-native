// ------------------------------------------------------------------
// render::Mesh.cpp - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "math/Constants.hpp"
#include "render/common/Mesh.hpp"

#include <cassert>
#include <numeric>

namespace stardazed {
namespace render {


void MeshDescriptor::createVertexBuffer(const AttributeList& attrs, size32_t itemCount) {
	vertexBuffer_ = std::make_unique<VertexBuffer>(attrs, itemCount);
}


math::AABB MeshDescriptor::calcAABB() const {
	math::AABB aabb;
	
	std::for_each(vertexBuffer().attrBegin<math::Vec3>(AttributeRole::Position), vertexBuffer().attrEnd<math::Vec3>(AttributeRole::Position),
		[&aabb](const math::Vec3& vec) {
			aabb.includeVector3(vec);
		});
	
	return aabb;
}


} // ns render
} // ns stardazed
