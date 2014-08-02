// ------------------------------------------------------------------
// render::Mesh.cpp - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "math/Constants.hpp"
#include "render/Mesh.hpp"

namespace stardazed {
namespace render {


void MeshDescriptor::calcVertexNormals() {
	vertexNormals.assign(vertexes.size(), { 0, 0, 1 });
	std::vector<float> usages(vertexes.size());
	
	for (const auto& face : faces) {
		auto lineA = vertexes[face[1]] - vertexes[face[0]];
		auto lineB = vertexes[face[2]] - vertexes[face[1]];

		if (math::nearEqual(length(lineA), 0.f) || math::nearEqual(length(lineB), 0.f))
			continue;

		auto faceNormal = math::normalize(math::cross(lineA, lineB));
		
		for (int fi = 0; fi < 3; ++fi) {
			auto fvi = face[fi];
			vertexNormals[fvi] = (vertexNormals[fvi] * usages[fvi] + faceNormal) / (usages[fvi] + 1.0f);

			usages[fvi] += 1.0f;
		}
	}
	
	std::for_each(begin(vertexNormals), end(vertexNormals), [](auto& vec) {
		math::normalizeInPlace(vec);
	});
}


void MeshDescriptor::calcVertexTangents() {
	// see http://www.terathon.com/code/tangent.html
}


math::AABB MeshDescriptor::calcAABB() const {
	math::AABB aabb;

	for (auto& vec : vertexes)
		aabb.includeVector3(vec);
	
	return aabb;
}


} // ns render
} // ns stardazed
