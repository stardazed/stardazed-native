// ------------------------------------------------------------------
// render::VertexDerivedData - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_VERTEXDERIVEDDATA_H
#define SD_RENDER_VERTEXDERIVEDDATA_H

#include "system/Config.hpp"
#include "math/Constants.hpp"
#include "math/Vector.hpp"
#include "render/common/VertexBuffer.hpp"
#include "render/common/IndexBuffer.hpp"

#include <vector>
#include <algorithm>

namespace stardazed {
namespace render {


void calcVertexNormals(VertexBuffer&, IndexBuffer&);
void calcVertexTangents(VertexBuffer&, IndexBuffer&);


template <typename VertIt, typename NormIt, typename FaceIt>
// requires RandomAccessIterator{VertIt}, RandomAccessIterator{NormIt}, RandomAccessIterator{FaceIt}
void calcVertexNormals(VertIt vertBegin, VertIt vertEnd, NormIt normBegin, NormIt normEnd, FaceIt faceBegin, FaceIt faceEnd) {
	auto vertexCount = std::distance(vertBegin, vertEnd);
	auto normalCount = std::distance(normBegin, normEnd);
	assert(vertexCount <= normalCount);
	
	std::fill(normBegin, normEnd, math::Vec3{ 0, 0, 1 });
	std::vector<float> usages(vertexCount);
	
	std::for_each(faceBegin, faceEnd,
		[=, &usages](const IndexBufferTriangleView::TriangleProxy& face) mutable {
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


template <typename VertIt, typename NormIt, typename TexIt, typename TanIt, typename FaceIt>
void calcVertexTangents(
						VertIt vertBegin, VertIt vertEnd,
						NormIt normBegin, NormIt normEnd,
						TexIt texBegin, TexIt texEnd,
						TanIt tanBegin, TanIt tanEnd,
						FaceIt faceBegin, FaceIt faceEnd
						)
{
	// adaptation of http://www.terathon.com/code/tangent.html
	// by Eric Lengyel
	
	using namespace sd::math;

	auto vertexCount = std::distance(vertBegin, vertEnd);
	assert(vertexCount <= std::distance(normBegin, normEnd));
	assert(vertexCount <= std::distance(texBegin, texEnd));
	assert(vertexCount <= std::distance(tanBegin, tanEnd));

	std::vector<math::Vec3> tanBuf(vertexCount * 2);
	auto tan1 = tanBuf.data();
	auto tan2 = tan1 + vertexCount;
	
	std::for_each(faceBegin, faceEnd,
		[=](const IndexBufferTriangleView::TriangleProxy& face) {
			auto a = face.a(),
				 b = face.b(),
				 c = face.c();
			
			auto &v1 = vertBegin[a],
				 &v2 = vertBegin[b],
				 &v3 = vertBegin[c];
			
			auto &w1 = texBegin[a],
				 &w2 = texBegin[b],
				 &w3 = texBegin[c];

			float x1 = v2.x - v1.x;
			float x2 = v3.x - v1.x;
			float y1 = v2.y - v1.y;
			float y2 = v3.y - v1.y;
			float z1 = v2.z - v1.z;
			float z2 = v3.z - v1.z;
			
			float s1 = w2.x - w1.x;
			float s2 = w3.x - w1.x;
			float t1 = w2.y - w1.y;
			float t2 = w3.y - w1.y;

			float r = 1.0f / (s1 * t2 - s2 * t1);
			math::Vec3 sdir((t2 * x1 - t1 * x2) * r,
							(t2 * y1 - t1 * y2) * r,
							(t2 * z1 - t1 * z2) * r);
			math::Vec3 tdir((s1 * x2 - s2 * x1) * r,
							(s1 * y2 - s2 * y1) * r,
							(s1 * z2 - s2 * z1) * r);
			
			tan1[a] += sdir;
			tan1[b] += sdir;
			tan1[c] += sdir;
			
			tan2[a] += tdir;
			tan2[b] += tdir;
			tan2[c] += tdir;
		});

	for (size32 ix = 0; ix < vertexCount; ++ix) {
		auto& n = normBegin[ix];
		auto& t = tan1[ix];
		
		// Gram-Schmidt orthogonalize, w component is handedness
		tanBegin[ix] = Vec4(
			normalize(t - n * dot(n, t)),
			(dot(cross(n, t), tan2[ix]) < 0.0f) ? -1.0f : 1.0f
		);
	}
}


} // ns render
} // ns stardazed

#endif
