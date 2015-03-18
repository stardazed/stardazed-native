// ------------------------------------------------------------------
// render::VertexDerivedData - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_VERTEXDERIVEDDATA_H
#define SD_RENDER_VERTEXDERIVEDDATA_H

#include "system/Config.hpp"
#include "math/Constants.hpp"
#include "math/Vector.hpp"

#include <vector>
#include <algorithm>

namespace stardazed {
namespace render {


template <typename VertIt, typename NormIt, typename FaceIt>
// requires RandomAccessIterator{VertIt}, RandomAccessIterator{NormIt}, RandomAccessIterator{FaceIt}
void calcVertexNormals(VertIt vertBegin, VertIt vertEnd, NormIt normBegin, NormIt normEnd, FaceIt faceBegin, FaceIt faceEnd) {
	auto vertexCount = std::distance(vertBegin, vertEnd);
	assert(vertexCount <= std::distance(normBegin, normEnd));
	
	std::fill(normBegin, normEnd, math::Vec3{ 0, 0, 1 });
	std::vector<float> usages(vertexCount);
	
	std::for_each(faceBegin, faceEnd,
		[=, &usages](const TriangleBuffer::TriangleProxy& face) mutable {
			auto lineA = vertBegin[face.b()] - vertBegin[face.a()];
			auto lineB = vertBegin[face.c()] - vertBegin[face.b()];

			if (math::nearEqual(length(lineA), 0.f) || math::nearEqual(length(lineB), 0.f))
				return;

			auto faceNormal = math::normalize(math::cross(lineA, lineB));

			for (int fi = 0; fi < 3; ++fi) {
				auto fvi = face.index(fi);
				normBegin[fvi] = (normBegin[fvi] * usages[fvi] + faceNormal) / (usages[fvi] + 1.0f);
				usages[fvi] += 1.0f;
			}
		});
	
	std::for_each(normBegin, normEnd, [](auto& vec) {
		math::normalizeInPlace(vec);
	});
}


//void calcVertexTangents() {
	// see http://www.terathon.com/code/tangent.html
//}


} // ns render
} // ns stardazed

#endif
