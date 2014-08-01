// ------------------------------------------------------------------
// geom::Generators.cpp - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "geom/Generators.hpp"
#include "math/Constants.hpp"
#include "math/Vector.hpp"
#include "math/Algorithm.hpp"


namespace stardazed {
namespace geom {
namespace gen {


render::MeshDescriptor plane() {
	render::MeshDescriptor mesh;
	
	mesh.vertexes.push_back({ -1, -1, 0 });
	mesh.vertexes.push_back({  1, -1, 0 });
	mesh.vertexes.push_back({  1,  1, 0 });
	mesh.vertexes.push_back({ -1,  1, 0 });
	
	mesh.faces.push_back({ 0, 2, 1 });
	mesh.faces.push_back({ 2, 0, 3 });
	
	mesh.calcVertexNormals();
	
	return mesh;
}



render::MeshDescriptor cube(const float diameter) {
	auto hd = diameter;
	render::MeshDescriptor mesh;
	
	// vertexes
	mesh.vertexes.push_back({ -hd, -hd, -hd });
	mesh.vertexes.push_back({  hd, -hd, -hd });
	mesh.vertexes.push_back({  hd,  hd, -hd });
	mesh.vertexes.push_back({ -hd,  hd, -hd });
	
	mesh.vertexes.push_back({ -hd, -hd,  hd });
	mesh.vertexes.push_back({  hd, -hd,  hd });
	mesh.vertexes.push_back({  hd,  hd,  hd });
	mesh.vertexes.push_back({ -hd,  hd,  hd });

	// faces
	mesh.faces.push_back({ 0, 2, 1 }); // front
	mesh.faces.push_back({ 2, 0, 3 });
	mesh.faces.push_back({ 1, 6, 5 }); // right
	mesh.faces.push_back({ 6, 1, 2 });
	mesh.faces.push_back({ 5, 7, 4 }); // back
	mesh.faces.push_back({ 7, 5, 6 });
	mesh.faces.push_back({ 4, 3, 0 }); // left
	mesh.faces.push_back({ 3, 4, 7 });
	mesh.faces.push_back({ 4, 1, 5 }); // top
	mesh.faces.push_back({ 1, 4, 0 });
	mesh.faces.push_back({ 3, 6, 2 }); // bottom
	mesh.faces.push_back({ 6, 3, 7 });
	
	mesh.calcVertexNormals();
	
	return mesh;
}



render::MeshDescriptor sphere(const int rows, const int segs, const float radius, float sliceFrom, float sliceTo) {
	assert(rows >= 2);
	assert(segs >= 4);
	sliceFrom = math::clamp(sliceFrom, 0.f, 1.f);
	sliceTo = math::clamp(sliceTo, 0.f, 1.f);
	assert(sliceTo > sliceFrom);

	const auto pi = math::Pi<float>, tau = math::Tau<float>;
	
	render::MeshDescriptor mesh;
	
	auto slice = sliceTo - sliceFrom,
		piFrom = sliceFrom * pi,
		piSlice = slice * pi;
	
	bool hasTopDisc = sliceFrom == 0.f,
		hasBottomDisc = sliceTo == 1.f;

	for (int row=0; row <= rows; ++row) {
		float y = std::cos(piFrom + (piSlice / rows) * row) * radius;
		float segRad = std::sin(piFrom + (piSlice / rows) * row) * radius;

		if (
			(hasTopDisc && row == 0) ||
			(hasBottomDisc && row == rows)
		) {
			// center top or bottom
			mesh.vertexes.push_back({ 0, y, 0 });
		}
		else {
			for (int seg=0; seg < segs; ++seg) {
				float x = std::sin((tau / segs) * seg) * segRad;
				float z = std::cos((tau / segs) * seg) * segRad;
				mesh.vertexes.push_back({ x, y, z });
			}
		}

		// construct row of faces
		if (row > 0) {
			int raix = static_cast<int>(mesh.vertexes.size()),
				rbix = static_cast<int>(mesh.vertexes.size()),
				ramul, rbmul;
			
			if (hasTopDisc && row == 1) {
				raix -= segs + 1;
				rbix -= segs;
				ramul = 0;
				rbmul = 1;
			}
			else if (hasBottomDisc && row == rows) {
				raix -= segs + 1;
				rbix -= 1;
				ramul = 1;
				rbmul = 0;
			}
			else {
				raix -= segs * 2;
				rbix -= segs;
				ramul = 1;
				rbmul = 1;
			}
			
			for (int seg=0; seg < segs; ++seg) {
				int ral = ramul * seg,
					rar = ramul * ((seg + 1) % segs),
					rbl = rbmul * seg,
					rbr = rbmul * ((seg + 1) % segs);
				
				mesh.faces.push_back({static_cast<uint16_t>(raix + ral), static_cast<uint16_t>(rbix + rbl), static_cast<uint16_t>(raix + rar)});
				mesh.faces.push_back({static_cast<uint16_t>(raix + rar), static_cast<uint16_t>(rbix + rbl), static_cast<uint16_t>(rbix + rbr)});
			}
		}
	}
	
	mesh.calcVertexNormals();
	return mesh;
}


} // ns gen
} // ns geom
} // ns stardazed
