// ------------------------------------------------------------------
// render::Mesh.cpp - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "render/Mesh.hpp"

#include <cassert>

namespace stardazed {
namespace render {


Mesh::Mesh(const MeshDescriptor& desc)
: vertexes_(desc.vertexes)
, vertexNormals_(desc.vertexNormals)
, vertexTangents_(desc.vertexTangents)
, vertexUVs_(desc.vertexUVs)
, faces_(desc.faces)
{
	assert(! vertexes_.empty());
	assert(! faces_.empty());

	if (vertexNormals_.empty())
		calcVertexNormals();
	if (vertexTangents_.empty())
		calcVertexTangents();
	
	assert(vertexes_.size() == vertexNormals_.size());
}


void Mesh::calcVertexNormals() {
	vertexNormals_.assign(vertexes_.size(), { 0, 0, 1 });
	std::vector<float> usages(vertexes_.size());
	
	for (const auto& face : faces_) {
		auto lineA = vertexes_[face[1]] - vertexes_[face[0]];
		auto lineB = vertexes_[face[2]] - vertexes_[face[1]];
		auto faceNormal = math::normalize(math::cross(lineA, lineB));
		
		for (int fi = 0; fi < 3; ++fi) {
			auto fvi = face[fi];
			vertexNormals_[fvi] = (vertexNormals_[fvi] * usages[fvi] + faceNormal) / (usages[fvi] + 1.0f);
			usages[fvi] += 1.0f;
		}
	}
	
	std::for_each(begin(vertexNormals_), end(vertexNormals_), [](auto& vec) {
		math::normalizeInPlace(vec);
	});
}


void Mesh::calcVertexTangents() {
}


} // ns render
} // ns stardazed
