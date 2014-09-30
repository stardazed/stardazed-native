// ------------------------------------------------------------------
// geom::Manipulators - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "geom/Manipulators.hpp"
#include "math/Constants.hpp"
#include "math/Vector.hpp"
#include "math/Algorithm.hpp"

namespace stardazed {
namespace geom {

using namespace render;
using namespace math;


MeshDescriptor& translate(MeshDescriptor& mesh, const Vec3& globalDelta) {
	std::transform(mesh.vertexes.begin(), mesh.vertexes.end(), mesh.vertexes.begin(), [&](const Vec3& vtx){
		return vtx + globalDelta;
	});

	return mesh;
}


MeshDescriptor& rotate(MeshDescriptor& mesh, const Quat& rotation) {
	auto vertexCount = mesh.vertexes.size();
	
	auto rotMat = rotation.toMatrix4();
	
	for (size_t ix=0; ix < vertexCount; ++ix) {
		mesh.vertexes[ix] = rotMat * mesh.vertexes[ix];
		mesh.vertexNormals[ix] = rotMat * mesh.vertexNormals[ix];
		// FIXME: tangents?
	}

	return mesh;
}


MeshDescriptor combine(const MeshDescriptor& m1, const MeshDescriptor& m2) {
	MeshDescriptor mm;

	auto stitch = [](const auto& v1, const auto& v2, auto& vOut) {
		vOut.resize(v1.size() + v2.size());
		std::copy(begin(v1), end(v1), begin(vOut));
		std::copy(begin(v2), end(v2), begin(vOut) + v1.size());
	};
	
	stitch(m1.vertexes,       m2.vertexes,       mm.vertexes);
	stitch(m1.vertexNormals,  m2.vertexNormals,  mm.vertexNormals);
	stitch(m1.vertexUVs,      m2.vertexUVs,      mm.vertexUVs);
	stitch(m1.vertexTangents, m2.vertexTangents, mm.vertexTangents);
	stitch(m1.faces,          m2.faces,          mm.faces);
	
	auto newFaces = begin(mm.faces) + m1.faces.size();
	std::for_each(newFaces, end(mm.faces), [vertexOffset = m1.vertexes.size()](Tri& face) {
		face[0] += vertexOffset;
		face[1] += vertexOffset;
		face[2] += vertexOffset;
	});
	
	return mm;
}


} // ns geom
} // ns stardazed
