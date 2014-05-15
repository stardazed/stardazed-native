// ------------------------------------------------------------------
// geom::Primitives.cpp - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "geom/Primitives.hpp"

namespace stardazed {
namespace geom {


void Mesh::calcVertexNormals() {
	vertexNormals.resize(vertexes.size(), { 0, 0, 1 });
	std::vector<float> usages(vertexes.size());
	
	for (const auto& face : faces) {
		auto lineA = vertexes[face[1]] - vertexes[face[0]];
		auto lineB = vertexes[face[2]] - vertexes[face[1]];
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


} // ns geom
} // ns stardazed
