// ------------------------------------------------------------------
// render::Mesh.cpp - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "math/Constants.hpp"
#include "render/common/Mesh.hpp"
#include "render/common/VertexDerivedData.hpp"

#include <cassert>
#include <numeric>

namespace stardazed {
namespace render {


MeshDescriptor::MeshDescriptor(const AttributeList& attrs)
: vertexBuffer(attrs)
{}


math::AABB MeshDescriptor::calcAABB() const {
	assert(vertexBuffer.attrByRole(AttributeRole::Position));

	math::AABB aabb;
	
	std::for_each(vertexBuffer.attrBegin<math::Vec3>(AttributeRole::Position), vertexBuffer.attrEnd<math::Vec3>(AttributeRole::Position),
		[&aabb](const math::Vec3& vec) {
			aabb.includeVector3(vec);
		});
	
	return aabb;
}


void MeshDescriptor::genVertexNormals() {
	auto posAttr = vertexBuffer.attrByRole(AttributeRole::Position),
		 normAttr = vertexBuffer.attrByRole(AttributeRole::Normal);

	assert(posAttr && normAttr);

	auto vertBegin = vertexBuffer.attrBegin<math::Vec3>(*posAttr),
		 vertEnd   = vertexBuffer.attrEnd<math::Vec3>(*posAttr);
	auto normBegin = vertexBuffer.attrBegin<math::Vec3>(*normAttr),
		 normEnd   = vertexBuffer.attrEnd<math::Vec3>(*normAttr);
	auto faceBegin = faces.begin(),
		 faceEnd   = faces.end();
	
	calcVertexNormals(vertBegin, vertEnd, normBegin, normEnd, faceBegin, faceEnd);
}


} // ns render
} // ns stardazed
