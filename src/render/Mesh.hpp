// ------------------------------------------------------------------
// render::Mesh - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_MESH_H
#define SD_RENDER_MESH_H

#include "math/Vector.hpp"

#include <array>
#include <vector>

namespace stardazed {
namespace render {


// exposition only, no real support for Tri32
using Tri16 = std::array<uint16_t, 3>;
using Tri32 = std::array<uint32_t, 3>;

using Tri = Tri16;


struct MeshDescriptor {
	// mandatory fields
	std::vector<math::Vec3> vertexes;
	std::vector<render::Tri> faces;
	
	// optional fields
	std::vector<math::Vec3> vertexNormals, vertexTangents;
	std::vector<math::Vec2> vertexUVs;
};


class Mesh {
public:
	Mesh(const MeshDescriptor& desc);
	
	const auto& vertexes() const { return vertexes_; }
	const auto& vertexNormals() const { return vertexNormals_; }
	const auto& vertexTangents() const { return vertexTangents_; }
	
	const auto& vertexUVs() const { return vertexUVs_; }
	
	const auto& faces() const { return faces_; }
	
private:
	void calcVertexNormals();
	void calcVertexTangents();

	std::vector<math::Vec3> vertexes_, vertexNormals_, vertexTangents_;
	std::vector<math::Vec2> vertexUVs_;
	std::vector<render::Tri> faces_;
};


class MeshBuffer {
	
};


} // ns render
} // ns stardazed

#endif
