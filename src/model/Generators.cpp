// ------------------------------------------------------------------
// model::Generators.cpp - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#include "model/Generators.hpp"
#include "math/Constants.hpp"
#include "math/Vector.hpp"
#include "math/Algorithm.hpp"

#include <cmath>
#include <vector>

namespace stardazed {
namespace model {
namespace gen {


//  ____  _
// |  _ \| | __ _ _ __   ___
// | |_) | |/ _` | '_ \ / _ \
// |  __/| | (_| | | | |  __/
// |_|   |_|\__,_|_| |_|\___|
//
Plane::Plane(float width, float height, float tileMaxDim, float tileUVStep, const PlaneYGenerator& yGen)
: yGen_(yGen)
{
	tilesWide_ = math::max(1.f, width / tileMaxDim);
	tilesHigh_ = math::max(1.f, height / tileMaxDim);
	tileDimX_  = width / tilesWide_;
	tileDimZ_  = height / tilesHigh_;
	
	tileUVStep_ = tileUVStep;
}


void Plane::generateImpl(const PositionAddFn& position, const FaceAddFn& face, const UVAddFn& uv) const {
	float halfWidth  = (tilesWide_ * tileDimX_) / 2,
		  halfHeight = (tilesHigh_ * tileDimZ_) / 2;

	// -- positions
	for (auto z = 0u; z <= tilesHigh_; ++z) {
		float posZ = -halfHeight + (z * tileDimZ_);
		
		for (auto x = 0u; x <= tilesWide_; ++x) {
			float posX = -halfWidth	+ (x * tileDimX_);
			position(posX, yGen_(posX, posZ), posZ);
			uv(
			   math::texCoord(((float)x / tilesWide_) * tileUVStep_),
			   math::texCoord(((float)z / tilesHigh_) * tileUVStep_)
			);
		}
	}

	// -- faces
	uint32 baseIndex = 0;
	uint32 vertexRowCount = tilesWide_ + 1;
	
	for (auto z = 0u; z < tilesHigh_; ++z) {
		for (auto x = 0u; x < tilesWide_; ++x) {
			face(
				baseIndex + x + 1,
				baseIndex + x + vertexRowCount,
				baseIndex + x + vertexRowCount + 1
			);
			face(
				baseIndex + x,
				baseIndex + x + vertexRowCount,
				baseIndex + x + 1
			);
		}

		baseIndex += vertexRowCount;
	}
}


//  ____
// | __ )  _____  __
// |  _ \ / _ \ \/ /
// | |_) | (_) >  <
// |____/ \___/_/\_\
//
void Box::generateImpl(const PositionAddFn& position, const FaceAddFn& face, const UVAddFn& uv) const {
	auto xh = xDiam_ / 2;
	auto yh = yDiam_ / 2;
	auto zh = zDiam_ / 2;
	uint32 curVtx = 0;
	
	// unique positions
	math::Vec3 p[8] = {
		{ -xh, -yh, -zh },
		{  xh, -yh, -zh },
		{  xh,  yh, -zh },
		{ -xh,  yh, -zh },

		{ -xh, -yh,  zh },
		{  xh, -yh,  zh },
		{  xh,  yh,  zh },
		{ -xh,  yh,  zh }
	};

	// topleft, topright, botright, botleft
	auto quad = [&](int a, int b, int c, int d) {
		position(p[a].x, p[a].y, p[a].z);
		position(p[b].x, p[b].y, p[b].z);
		position(p[c].x, p[c].y, p[c].z);
		position(p[d].x, p[d].y, p[d].z);

		// each cube quad shows texture fully
		uv(0, 0);
		uv(1, 0);
		uv(1, 1);
		uv(0, 1);

		// ccw faces
		face(curVtx, curVtx + 1, curVtx + 2);
		face(curVtx + 2, curVtx + 3, curVtx);
		
		curVtx += 4;
	};

	quad(3, 2, 1, 0); // front
	quad(7, 3, 0, 4); // left
	quad(6, 7, 4, 5); // back
	quad(2, 6, 5, 1); // right
	quad(7, 6, 2, 3); // top
	quad(5, 4, 0, 1); // bottom
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


void Arc::generateImpl(const PositionAddFn& position, const FaceAddFn& face, const UVAddFn&) const {
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

	// -- positions
	for (int step=0; step < angleVerts; ++step) {
		auto ang = angA + (step * angStep);
		std::for_each(begin(radii), end(radii), [ang, &position](float r) {
			position(r * math::cos(ang), 0, r * math::sin(ang));
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
Sphere::Sphere(float radius, int rows, int segs, float sliceFrom, float sliceTo)
: rows_(rows)
, segs_(segs)
, radius_(radius)
, sliceFrom_(math::clamp(sliceFrom, 0.f, 1.f))
, sliceTo_(math::clamp(sliceTo, 0.f, 1.f))
{
	assert(rows_ >= 2);
	assert(segs_ >= 4);
	assert(sliceTo_ > sliceFrom_);
}


void Sphere::generateImpl(const PositionAddFn& position, const FaceAddFn& face, const UVAddFn& uv) const {
	using math::Pi;
	using math::Tau;
	
	float slice = sliceTo_ - sliceFrom_,
		  piFrom = sliceFrom_ * Pi.val(),
		  piSlice = slice * Pi.val();

	int vix = 0;
	
	for (int row=0; row <= rows_; ++row) {
		float y = std::cos(piFrom + (piSlice / rows_) * row) * radius_;
		float segRad = std::sin(piFrom + (piSlice / rows_) * row) * radius_;
		float texV = sliceFrom_ + (((float)row / rows_) * slice);
		
		for (int seg=0; seg <= segs_; ++seg) {
			float x = math::sin((Tau / segs_) * seg) * segRad;
			float z = math::cos((Tau / segs_) * seg) * segRad;
			float texU = (float)seg / segs_;

			position(x, y, z);
			uv(texU, texV);
			++vix;
		}
		
		// construct row of faces
		if (row > 0) {
			int raix = vix - ((segs_ + 1) * 2),
				rbix = vix - (segs_ + 1);

			for (int seg=1; seg <= segs_; ++seg) {
				int rl = seg,
					rr = rl == segs_ ? 1 : seg + 1;
				
				if (row > 1)
					face(raix + rl, rbix + rl, raix + rr);
				if (row < rows_)
					face(raix + rr, rbix + rl, rbix + rr);
			}
		}
	}
}


} // ns gen
} // ns model
} // ns stardazed
