// ------------------------------------------------------------------
// render::Mesh.cpp - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#include "math/Constants.hpp"
#include "render/common/Mesh.hpp"
#include "render/common/VertexDerivedData.hpp"

#include <algorithm>

namespace stardazed {
namespace render {


MeshData::MeshData(const VertexAttributeList& attrs) {
	vertexBuffers.emplaceBack(attrs);
}


VertexBuffer& MeshData::primaryVertexBuffer() {
	assert(vertexBuffers.count() > 0);
	return vertexBuffers.front();
}


MeshDescriptor MeshData::defaultDescriptor() const {
	MeshDescriptor md;

	uint32 curAttrIndex = 0;

	for (auto& vertexBuffer : vertexBuffers) {
		VertexBufferBinding vbb;
		vbb.vertexBuffer = &vertexBuffer;
		vbb.baseAttributeIndex = curAttrIndex;
		curAttrIndex += vertexBuffer.attributeCount();
		
		md.vertexBindings.append(vbb);
	}
	
	md.indexBinding.indexBuffer = &indexBuffer;
	
	md.faceGroups = faceGroups;

	return md;
}


void MeshData::genVertexNormals() {
	for (auto& vertexBuffer : vertexBuffers) {
		auto posAttr = vertexBuffer.attrByRole(VertexAttributeRole::Position),
			 normAttr = vertexBuffer.attrByRole(VertexAttributeRole::Normal);
		
		if (posAttr && normAttr) {
			calcVertexNormals(vertexBuffer, indexBuffer);
		}
	}
}


void MeshData::genVertexTangents() {
	for (auto& vertexBuffer : vertexBuffers) {
		auto posAttr  = vertexBuffer.attrByRole(VertexAttributeRole::Position),
			 normAttr = vertexBuffer.attrByRole(VertexAttributeRole::Normal),
			 texAttr  = vertexBuffer.attrByRole(VertexAttributeRole::UV),
			 tanAttr  = vertexBuffer.attrByRole(VertexAttributeRole::Tangent);
	
		if (posAttr && normAttr && texAttr && tanAttr) {
			calcVertexTangents(vertexBuffer, indexBuffer);
		}
	}
}


} // ns render
} // ns stardazed
