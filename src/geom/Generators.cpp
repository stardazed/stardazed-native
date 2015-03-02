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


//  ____  _
// |  _ \| | __ _ _ __   ___
// | |_) | |/ _` | '_ \ / _ \
// |  __/| | (_| | | | |  __/
// |_|   |_|\__,_|_| |_|\___|
//
Plane::Plane(float width, float height, float tileMaxDim, const PlaneYGenerator& yGen)
: yGen_(yGen)
{
	tilesWide_ = math::max(1.0f, width / tileMaxDim),
	tilesHigh_ = math::max(1.0f, height / tileMaxDim),
	tileDimX_  = width / tilesWide_,
	tileDimZ_  = height / tilesHigh_;
}


void Plane::generateImpl(const VertexAddFn& vertex, const FaceAddFn& face) const {
	float halfWidth  = (tilesWide_ * tileDimX_) / 2,
		  halfHeight = (tilesHigh_ * tileDimZ_) / 2;
	
	// -- vertexes
	for (auto z = 0u; z <= tilesHigh_; ++z) {
		float posZ = -halfHeight + (z * tileDimZ_);
		
		for (auto x = 0u; x <= tilesWide_; ++x) {
			float posX = -halfWidth	+ (x * tileDimX_);
			vertex(posX, yGen_(posX, posZ), posZ);
		}
	}
	
	// -- faces
	uint32 baseIndex = 0;
	uint32 vertexRowCount = tilesWide_ + 1;
	
	for (auto z = 0u; z < tilesHigh_; ++z) {
		for (auto x = 0u; x < tilesWide_; ++x) {
			face(
				baseIndex + 1,
				baseIndex + vertexRowCount,
				baseIndex + vertexRowCount + 1
			);
			face(
				baseIndex,
				baseIndex + vertexRowCount,
				baseIndex + 1
			);
		}
		
		baseIndex += vertexRowCount;
	}
}


//   ____      _
//  / ___|   _| |__   ___
// | |  | | | | '_ \ / _ \
// | |__| |_| | |_) |  __/
//  \____\__,_|_.__/ \___|
//
void Cube::generateImpl(const VertexAddFn& vertex, const FaceAddFn& face) const {
	auto hd = diameter_ / 2.f;
	
	// vertexes
	vertex(-hd, -hd, -hd);
	vertex( hd, -hd, -hd);
	vertex( hd,  hd, -hd);
	vertex(-hd,  hd, -hd);
	
	vertex(-hd, -hd,  hd);
	vertex( hd, -hd,  hd);
	vertex( hd,  hd,  hd);
	vertex(-hd,  hd,  hd);
	
	// faces
	face( 0, 2, 1 ); // front
	face( 2, 0, 3 );
	face( 1, 6, 5 ); // right
	face( 6, 1, 2 );
	face( 5, 7, 4 ); // back
	face( 7, 5, 6 );
	face( 4, 3, 0 ); // left
	face( 3, 4, 7 );
	face( 4, 1, 5 ); // top
	face( 1, 4, 0 );
	face( 3, 6, 2 ); // bottom
	face( 6, 3, 7 );
}


//     _
//    / \   _ __ ___
//   / _ \ | '__/ __|
//  / ___ \| | | (__
// /_/   \_\_|  \___|
//
Arc::Arc(float minRadius, float maxRadius, int radiusSteps,
		 math::Angle fromAng, math::Angle toAng, int angleSteps)
: minRadius_(minRadius)
, maxRadius_(maxRadius)
, fromAng_(fromAng)
, toAng_(toAng)
, radiusSteps_(radiusSteps)
, angleSteps_(angleSteps)
{}


void Arc::generateImpl(const VertexAddFn& vertex, const FaceAddFn& face) const {
	using math::Radians;
	using math::Tau;
	
	// -- arc shape
	Radians angA = fromAng_.rad(), angB = toAng_.rad();
	if (angB < angA)
		angB += Tau;
	Radians angStep = (angB - angA) / angleSteps_;
	
	auto radiusVerts = radiusSteps_ + 1;
	auto angleVerts  = angleSteps_ + 1;
	
	// -- radius steps
	std::vector<float> radii(radiusVerts);
	float radStep = (maxRadius_ - minRadius_) / radiusSteps_;
	std::generate(begin(radii), end(radii), [this, radStep, i=0]() mutable {
		return minRadius_ + (i++ * radStep);
	});

	// -- vertexes
	for (int step=0; step < angleVerts; ++step) {
		auto ang = angA + (step * angStep);
		std::for_each(begin(radii), end(radii), [ang, &vertex](float r) {
			vertex(r * math::cos(ang), 0, r * math::sin(ang));
		});
	}
	
	// -- faces
	uint32 vix = 0;
	for (int seg=0; seg < angleSteps_; ++seg) {
		for (int track=0; track < radiusSteps_; ++track) {
			face(
				vix + track,
				vix + track + 1,
				vix + track + 1 + radiusVerts
			);
			face(
				vix + track,
				vix + track + 1 + radiusVerts,
				vix + track + radiusVerts
			);
		}
		
		vix += radiusVerts;
	}
}


//  ____        _
// / ___| _ __ | |__   ___ _ __ ___
// \___ \| '_ \| '_ \ / _ \ '__/ _ \
//  ___) | |_) | | | |  __/ | |  __/
// |____/| .__/|_| |_|\___|_|  \___|
//       |_|
Sphere::Sphere(int rows, int segs, float radius, float sliceFrom, float sliceTo)
: rows_(rows)
, segs_(segs)
, radius_(radius)
, sliceFrom_(math::clamp(sliceFrom, 0.f, 1.f))
, sliceTo_(math::clamp(sliceTo, 0.f, 1.f))
{
	assert(rows >= 2);
	assert(segs >= 4);
	assert(sliceTo > sliceFrom);
}


void Sphere::generateImpl(const VertexAddFn& vertex, const FaceAddFn& face) const {
	using math::Pi;
	using math::Tau;
	
	float slice = sliceTo_ - sliceFrom_,
		  piFrom = sliceFrom_ * Pi.val(),
		  piSlice = slice * Pi.val();

	int vix = 0;
	
	for (int row=0; row <= rows_; ++row) {
		float y = std::cos(piFrom + (piSlice / rows_) * row) * radius_;
		float segRad = std::sin(piFrom + (piSlice / rows_) * row) * radius_;
		
		if (
			(hasTopDisc() && row == 0) ||
			(hasBottomDisc() && row == rows_)
		) {
			// center top or bottom
			vertex(0, y, 0);
			++vix;
		}
		else {
			for (int seg=0; seg < segs_; ++seg) {
				float x = math::sin((Tau / segs_) * seg) * segRad;
				float z = math::cos((Tau / segs_) * seg) * segRad;
				vertex(x, y, z);
				++vix;
			}
		}
		
		// construct row of faces
		if (row > 0) {
			int raix = vix,
				rbix = vix,
				ramul, rbmul;
			
			if (hasTopDisc() && row == 1) {
				raix -= segs_ + 1;
				rbix -= segs_;
				ramul = 0;
				rbmul = 1;
			}
			else if (hasBottomDisc() && row == rows_) {
				raix -= segs_ + 1;
				rbix -= 1;
				ramul = 1;
				rbmul = 0;
			}
			else {
				raix -= segs_ * 2;
				rbix -= segs_;
				ramul = 1;
				rbmul = 1;
			}
			
			for (int seg=0; seg < segs_; ++seg) {
				int ral = ramul * seg,
					rar = ramul * ((seg + 1) % segs_),
					rbl = rbmul * seg,
					rbr = rbmul * ((seg + 1) % segs_);
				
				face(raix + ral, rbix + rbl, raix + rar);
				face(raix + rar, rbix + rbl, rbix + rbr);
			}
		}
	}
}


} // ns gen
} // ns geom
} // ns stardazed
