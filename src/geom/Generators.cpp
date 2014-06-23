// ------------------------------------------------------------------
// geom::Generators.cpp - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "geom/Generators.hpp"
#include "math/Constants.hpp"
#include "math/Vector.hpp"


namespace stardazed {
namespace geom {
namespace gen {


render::Mesh plane(math::Vec3 origin, math::Vec2 xx) {
	render::Mesh mesh;
	
	mesh.vertexes.push_back({ -1, -1, 0 });
	mesh.vertexes.push_back({  1, -1, 0 });
	mesh.vertexes.push_back({  1,  1, 0 });
	mesh.vertexes.push_back({ -1,  1, 0 });
	
	mesh.faces.push_back({ 0, 1, 2 });
	mesh.faces.push_back({ 2, 3, 0 });
	
	mesh.calcVertexNormals();
	
	return mesh;
}



render::Mesh cube(const float diameter) {
	auto hd = diameter;
	render::Mesh mesh;
	
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
	mesh.faces.push_back({ 0, 1, 2 }); // front
	mesh.faces.push_back({ 2, 3, 0 });
	mesh.faces.push_back({ 1, 5, 6 }); // right
	mesh.faces.push_back({ 6, 2, 1 });
	mesh.faces.push_back({ 5, 4, 7 }); // back
	mesh.faces.push_back({ 7, 6, 5 });
	mesh.faces.push_back({ 4, 0, 3 }); // left
	mesh.faces.push_back({ 3, 7, 4 });
	mesh.faces.push_back({ 4, 5, 1 }); // top
	mesh.faces.push_back({ 1, 0, 4 });
	mesh.faces.push_back({ 3, 2, 6 }); // bottom
	mesh.faces.push_back({ 6, 7, 3 });
	
	mesh.calcVertexNormals();
	
	return mesh;
}



render::Mesh sphere(const int rows, const int segs, const float radius, const float sliceFrom, const float sliceTo) {
	const auto pi = math::Pi<float>, tau = math::Tau<float>;
	
	render::Mesh mesh;
	uint16_t vix = 0; // current vertex index
	
	auto slice = sliceTo - sliceFrom,
	piFrom = sliceFrom * pi,
	piSlice = slice * pi;
	
	for (int row=0; row < rows; ++row) {
		float y1 = std::cos(piFrom + (piSlice / rows) * row) * radius;
		float y2 = std::cos(piFrom + (piSlice / rows) * (row + 1)) * radius;
		float segRad1 = std::sin(piFrom + (piSlice / rows) * row) * radius;
		float segRad2 = std::sin(piFrom + (piSlice / rows) * (row + 1)) * radius;
		
		for (int seg=0; seg < segs; ++seg) {
			float x1L = std::sin((tau / segs) * (seg + 1)) * segRad1;
			float z1L = std::cos((tau / segs) * (seg + 1)) * segRad1;
			float x1R = std::sin((tau / segs) * seg) * segRad1;
			float z1R = std::cos((tau / segs) * seg) * segRad1;
			
			float x2L = std::sin((tau / segs) * (seg + 1)) * segRad2;
			float z2L = std::cos((tau / segs) * (seg + 1)) * segRad2;
			float x2R = std::sin((tau / segs) * seg) * segRad2;
			float z2R = std::cos((tau / segs) * seg) * segRad2;
			
			// add 4 corners of 1 segment as vertexes
			mesh.vertexes.push_back({ x1L, y1, z1L }); // top left
			mesh.vertexes.push_back({ x1R, y1, z1R }); // top right
			mesh.vertexes.push_back({ x2R, y2, z2R }); // bottom right
			mesh.vertexes.push_back({ x2L, y2, z2L }); // bottom left
			
			// add the two triangle faces
			mesh.faces.push_back({ static_cast<uint16_t>(vix + 0), static_cast<uint16_t>(vix + 1), static_cast<uint16_t>(vix + 2) });
			mesh.faces.push_back({ static_cast<uint16_t>(vix + 2), static_cast<uint16_t>(vix + 3), static_cast<uint16_t>(vix + 0) });
			
			vix += 4;
		}
	}
	
	mesh.calcVertexNormals();
	
	return mesh;
}


} // ns gen
} // ns geom
} // ns stardazed
