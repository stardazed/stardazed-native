// ------------------------------------------------------------------
// geom::Generators - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_GEOM_GENERATORS_H
#define SD_GEOM_GENERATORS_H

#include "system/Config.hpp"
#include "math/Angle.hpp"
#include "render/common/Mesh.hpp"

#include <type_traits>
#include <functional>

namespace stardazed {
namespace geom {
namespace gen {


using VertexAddFn = std::function<void(float, float, float)>;
using FaceAddFn = std::function<void(uint32_t, uint32_t, uint32_t)>;


template <typename MeshImpl>
class MeshGenerator {
public:
	virtual ~MeshGenerator() = default;
	
	virtual size32 vertexCount() const = 0;
	virtual size32 faceCount() const = 0;

	template <typename VertexIter, typename FaceIter>
	void generate(VertexIter vertexBegin, FaceIter faceBegin) const {
		using TriangleIndexType = std::remove_reference_t<decltype((*faceBegin)[0])>;
		
		// vertex is convertible to a VertexAddFn
		auto vertex = [vit = vertexBegin](float x, float y, float z) mutable {
			*vit++ = { x, y, z };
		};
		
		// face is convertible to a FaceAddFn
		auto face = [fit = faceBegin](uint32 a, uint32 b, uint32 c) mutable {
			*fit++ = {
				static_cast<TriangleIndexType>(a),
				static_cast<TriangleIndexType>(b),
				static_cast<TriangleIndexType>(c)
			};
		};
		
		static_cast<const MeshImpl*>(this)->generateImpl(vertex, face);
	}
};


using PlaneYGenerator = std::function<float(float, float)>;

class Plane : public MeshGenerator<Plane> {
	size32 tilesWide_, tilesHigh_, tileDimX_, tileDimZ_;
	PlaneYGenerator yGen_;

public:
	Plane(float width, float height, float tileMaxDim, const PlaneYGenerator& yGen);

	// delegating constructor that just yields 0.0 for all y-coords
	Plane(float width, float height, float tileMaxDim)
	: Plane(width, height, tileMaxDim, [](float,float){ return 0.f; }) {}

	size32 vertexCount() const override { return (tilesWide_ + 1) * (tilesHigh_ + 1); }
	size32 faceCount() const override { return 2 * tilesWide_ * tilesHigh_; }
	
	void generateImpl(const VertexAddFn&, const FaceAddFn&) const;
};


class Cube : public MeshGenerator<Cube> {
	float diameter_;
	
public:
	Cube(float diameter)
	: diameter_(diameter)
	{}
	
	size32 vertexCount() const override { return 8; }
	size32 faceCount() const override { return 12; }
	
	void generateImpl(const VertexAddFn&, const FaceAddFn&) const;
};


class Arc : public MeshGenerator<Arc> {
	float minRadius_, maxRadius_;
	math::Angle fromAng_, toAng_;
	int radiusSteps_, angleSteps_;

public:
	Arc(float minRadius, float maxRadius, int radiusSteps,
		math::Angle fromAng, math::Angle toAng, int angleSteps);
	
	size32 vertexCount() const {
		return (radiusSteps_ + 1) * (angleSteps_ + 1);
	}
	
	size32 faceCount() const {
		return (radiusSteps_ * 2) * angleSteps_;
	}
	
	void generateImpl(const VertexAddFn&, const FaceAddFn&) const;
};


class Sphere : public MeshGenerator<Sphere> {
	int rows_, segs_;
	float radius_, sliceFrom_, sliceTo_;
	
	bool hasTopDisc() const { return sliceFrom_ == 0.f; }
	bool hasBottomDisc() const { return sliceTo_ == 1.f; }

public:
	Sphere(int rows = 20, int segs = 30, float radius = 1.0f,
		   float sliceFrom = 0.0f, float sliceTo = 1.0f);
	
	size32 vertexCount() const override {
		size32 vc = segs_ * (rows_ - 1);
		if (hasTopDisc()) ++vc;
		if (hasBottomDisc()) ++vc;
		return vc;
	}

	size32 faceCount() const override {
		return 2u * segs_ * rows_;
	}
	
	void generateImpl(const VertexAddFn&, const FaceAddFn&) const;
};


template <typename Gen, typename... Args>
render::MeshDescriptor basic(Args&&... args) {
	using namespace render;
	
	MeshDescriptor mesh({
		{ { render::fieldVec3(), "position" }, AttributeRole::Position },
		{ { render::fieldVec3(), "normal" }, AttributeRole::Normal }
	});
	
	auto generator = Gen(std::forward<Args>(args)...);

	mesh.vertexBuffer.allocate<OwnedBufferStorage>(generator.vertexCount());
	mesh.faces.resize(generator.faceCount());

	auto posIter = mesh.vertexBuffer.attrBegin<math::Vec3>(AttributeRole::Position);
	generator.generate(posIter, mesh.faces.begin());
	
	mesh.genVertexNormals();
	return mesh;
}


// ---- Convenience methods to create a basic MeshDescriptor for each of the primitves

template <typename... Args>
render::MeshDescriptor plane(Args&&... args) { return basic<Plane>(std::forward<Args>(args)...); }

template <typename... Args>
render::MeshDescriptor cube(Args&&... args) { return basic<Cube>(std::forward<Args>(args)...); }

template <typename... Args>
render::MeshDescriptor arc(Args&&... args) { return basic<Arc>(std::forward<Args>(args)...); }

template <typename... Args>
render::MeshDescriptor sphere(Args&&... args) { return basic<Sphere>(std::forward<Args>(args)...); }


} // ns gen
} // ns geom
} // ns stardazed


#endif
