// ------------------------------------------------------------------
// render::VertexDerivedData.cpp - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#include "render/common/VertexDerivedData.hpp"

namespace stardazed {
namespace render {


void calcVertexNormals(VertexBuffer& vertexBuffer, IndexBuffer& indexBuffer) {
	auto posAttr = vertexBuffer.attrByRole(VertexAttributeRole::Position),
		 normAttr = vertexBuffer.attrByRole(VertexAttributeRole::Normal);

	assert(posAttr && normAttr);
	
	IndexBufferTriangleView triView{ indexBuffer };

	auto vertBegin = vertexBuffer.attrBegin<math::Vec3>(*posAttr),
		 vertEnd   = vertexBuffer.attrEnd<math::Vec3>(*posAttr);
	auto normBegin = vertexBuffer.attrBegin<math::Vec3>(*normAttr),
		 normEnd   = vertexBuffer.attrEnd<math::Vec3>(*normAttr);
	auto faceBegin = triView.begin(),
		 faceEnd   = triView.end();

	calcVertexNormals(vertBegin, vertEnd, normBegin, normEnd, faceBegin, faceEnd);
}


void calcVertexTangents(VertexBuffer& vertexBuffer, IndexBuffer& indexBuffer) {
	auto posAttr  = vertexBuffer.attrByRole(VertexAttributeRole::Position),
		 normAttr = vertexBuffer.attrByRole(VertexAttributeRole::Normal),
		 texAttr  = vertexBuffer.attrByRole(VertexAttributeRole::UV),
		 tanAttr  = vertexBuffer.attrByRole(VertexAttributeRole::Tangent);
	
	IndexBufferTriangleView triView{ indexBuffer };

	assert(posAttr && normAttr && texAttr && tanAttr);
	
	auto vertBegin = vertexBuffer.attrBegin<math::Vec3>(*posAttr),
		 vertEnd   = vertexBuffer.attrEnd<math::Vec3>(*posAttr);
	auto normBegin = vertexBuffer.attrBegin<math::Vec3>(*normAttr),
		 normEnd   = vertexBuffer.attrEnd<math::Vec3>(*normAttr);
	auto texBegin  = vertexBuffer.attrBegin<math::Vec3>(*texAttr),
		 texEnd    = vertexBuffer.attrEnd<math::Vec3>(*texAttr);
	auto tanBegin  = vertexBuffer.attrBegin<math::Vec4>(*tanAttr),
		 tanEnd    = vertexBuffer.attrEnd<math::Vec4>(*tanAttr);
	auto faceBegin = triView.begin(),
		 faceEnd   = triView.end();
	
	calcVertexTangents(vertBegin, vertEnd, normBegin, normEnd,
					   texBegin, texEnd, tanBegin, tanEnd,
					   faceBegin, faceEnd);
}


} // ns render
} // ns stardazed
