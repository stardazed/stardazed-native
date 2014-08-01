// ------------------------------------------------------------------
// render::Mesh.cpp - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "render/Mesh.hpp"

#include <cassert>
#include <limits>

namespace stardazed {
namespace render {


void MeshDescriptor::calcVertexNormals() {
	vertexNormals.assign(vertexes.size(), { 0, 0, 1 });
	std::vector<float> usages(vertexes.size());
	
	float epsilon = std::numeric_limits<float>::epsilon();
	
	for (const auto& face : faces) {
		auto lineA = vertexes[face[1]] - vertexes[face[0]];
		auto lineB = vertexes[face[2]] - vertexes[face[1]];

		if (length(lineA) <= epsilon || length(lineB) <= epsilon)
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


} // ns render
} // ns stardazed
