// ------------------------------------------------------------------
// render::Mesh.cpp - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "math/Constants.hpp"
#include "render/common/Mesh.hpp"
#include "render/common/VertexDerivedData.hpp"

#include <algorithm>

namespace stardazed {
namespace render {


MeshDescriptor::MeshDescriptor(const AttributeList& attrs)
: vertexBuffer(attrs)
{}


math::AABB MeshDescriptor::calcAABB() const {
	auto posAttr = vertexBuffer.attrByRole(AttributeRole::Position);
	assert(posAttr);

	math::AABB aabb;
	
	std::for_each(vertexBuffer.attrBegin<math::Vec3>(*posAttr), vertexBuffer.attrEnd<math::Vec3>(*posAttr),
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


void MeshDescriptor::genVertexTangents() {
	auto posAttr  = vertexBuffer.attrByRole(AttributeRole::Position),
		 normAttr = vertexBuffer.attrByRole(AttributeRole::Normal),
		 texAttr  = vertexBuffer.attrByRole(AttributeRole::UV),
		 tanAttr  = vertexBuffer.attrByRole(AttributeRole::Tangent);
	
	assert(posAttr && normAttr && texAttr && tanAttr);
	
	auto vertBegin = vertexBuffer.attrBegin<math::Vec3>(*posAttr),
		 vertEnd   = vertexBuffer.attrEnd<math::Vec3>(*posAttr);
	auto normBegin = vertexBuffer.attrBegin<math::Vec3>(*normAttr),
		 normEnd   = vertexBuffer.attrEnd<math::Vec3>(*normAttr);
	auto texBegin  = vertexBuffer.attrBegin<math::Vec3>(*texAttr),
		 texEnd    = vertexBuffer.attrEnd<math::Vec3>(*texAttr);
	auto tanBegin  = vertexBuffer.attrBegin<math::Vec4>(*tanAttr),
		 tanEnd    = vertexBuffer.attrEnd<math::Vec4>(*tanAttr);
	auto faceBegin = faces.begin(),
		 faceEnd   = faces.end();
	
	calcVertexTangents(vertBegin, vertEnd, normBegin, normEnd,
					   texBegin, texEnd, tanBegin, tanEnd,
					   faceBegin, faceEnd);
}


} // ns render
} // ns stardazed
