// ------------------------------------------------------------------
// model::Generators - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_MODEL_GENERATORS_H
#define SD_MODEL_GENERATORS_H

#include "system/Config.hpp"
#include "math/Angle.hpp"
#include "render/common/Mesh.hpp"

#include <type_traits>
#include <functional>
#include <initializer_list>

namespace stardazed {
namespace model {
namespace gen {


using PositionAddFn = std::function<void(float, float, float)>;
using FaceAddFn = std::function<void(uint32, uint32, uint32)>;
using UVAddFn = std::function<void(float, float)>;


namespace detail {
	
	struct BlackHole {
		constexpr BlackHole& operator *() { return *this; }
		constexpr BlackHole& operator ++() { return *this; }
		constexpr BlackHole& operator ++(int) { return *this; }
		
		template <typename T>
		constexpr BlackHole& operator =(const T&) { return *this; }
		template <typename T>
		constexpr BlackHole& operator =(std::initializer_list<T>) { return *this; }
	};
	
} // ns detail


template <typename MeshImpl>
class MeshGenerator {
public:
	virtual ~MeshGenerator() = default;
	
	// both position and UV attributes have vertexCount elements
	virtual size32 vertexCount() const = 0;
	// each face is a triangle (3 indexes into the vertex arrays)
	virtual size32 faceCount() const = 0;

	template <typename PositionIter, typename FaceIter, typename UVIter>
	void generate(PositionIter positionBegin, FaceIter faceBegin, UVIter uvBegin) const {
		// vertex is convertible to a VertexAddFn
		auto position = [pit = positionBegin](float x, float y, float z) mutable {
			*pit++ = { x, y, z };
		};
		
		// face is convertible to a FaceAddFn
		auto face = [fit = faceBegin](uint32 a, uint32 b, uint32 c) mutable {
			*fit++ = render::Triangle{ a, b, c };
		};

		// uv is convertible to an UVAddFn
		auto uv = [uvit = uvBegin](float u, float v) mutable {
			*uvit++ = { u, v };
		};
		
		static_cast<const MeshImpl*>(this)->generateImpl(position, face, uv);
	}

	template <typename PositionIter, typename FaceIter>
	void generate(PositionIter positionBegin, FaceIter faceBegin) const {
		// non-UV version just passes a blackhole iterator
		generate(positionBegin, faceBegin, detail::BlackHole{});
	}
};


using PlaneYGenerator = std::function<float(float, float)>;

class Plane : public MeshGenerator<Plane> {
	size32 tilesWide_, tilesHigh_;
	float tileDimX_, tileDimZ_, tileUVStep_;
	PlaneYGenerator yGen_;

public:
	Plane(float width, float height, float tileMaxDim, float tileUVStep, const PlaneYGenerator& yGen);

	// delegating constructor that just yields 0.0 for all y-coords
	Plane(float width, float height, float tileMaxDim, float tileUVStep = 1.0)
	: Plane(width, height, tileMaxDim, tileUVStep, [](float,float){ return 0.f; }) {}

	size32 vertexCount() const override { return (tilesWide_ + 1) * (tilesHigh_ + 1); }
	size32 faceCount() const override { return 2 * tilesWide_ * tilesHigh_; }
	
	void generateImpl(const PositionAddFn&, const FaceAddFn&, const UVAddFn&) const;
};


class Box : public MeshGenerator<Box> {
	float xDiam_, yDiam_, zDiam_;
	
public:
	Box(float diameter)
	: xDiam_(diameter), yDiam_(diameter), zDiam_(diameter)
	{}
	Box(float xDiam, float yDiam, float zDiam)
	: xDiam_(xDiam), yDiam_(yDiam), zDiam_(zDiam)
	{}
	
	size32 vertexCount() const override { return 24; }
	size32 faceCount() const override { return 12; }
	
	void generateImpl(const PositionAddFn&, const FaceAddFn&, const UVAddFn&) const;
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
	
	void generateImpl(const PositionAddFn&, const FaceAddFn&, const UVAddFn&) const;
};


class Sphere : public MeshGenerator<Sphere> {
	int rows_, segs_;
	float radius_, sliceFrom_, sliceTo_;
	
public:
	Sphere(float radius = 1.0f, int rows = 20, int segs = 30,
		   float sliceFrom = 0.0f, float sliceTo = 1.0f);
	
	size32 vertexCount() const override {
		return (segs_ + 1) * (rows_ + 1);
	}

	size32 faceCount() const override {
		return 2 * (segs_ + 1) * (rows_ + 1);
	}
	
	void generateImpl(const PositionAddFn&, const FaceAddFn&, const UVAddFn&) const;
};


template <typename Gen, typename... Args>
void into(render::MeshData& mesh, Args&&... args) {
	using namespace render;
	
	auto generator = Gen(std::forward<Args>(args)...);
	auto& vertexBuffer = mesh.primaryVertexBuffer();
	
	vertexBuffer.allocate(generator.vertexCount());
	auto indexElementType = minimumIndexElementTypeForVertexCount(generator.vertexCount());
	mesh.indexBuffer.allocate(PrimitiveType::Triangle, indexElementType, generator.faceCount());
	
	auto texAttr = vertexBuffer.attrByRole(VertexAttributeRole::UV);
	auto tanAttr = vertexBuffer.attrByRole(VertexAttributeRole::Tangent);
	
	auto posIter = vertexBuffer.attrBegin<math::Vec3>(VertexAttributeRole::Position);

	IndexBufferTriangleView triView{ mesh.indexBuffer };
	if (texAttr)
		generator.generate(posIter, triView.begin(), vertexBuffer.attrBegin<math::Vec2>(*texAttr));
	else
		generator.generate(posIter, triView.begin());
	
	mesh.genVertexNormals();
	if (tanAttr)
		mesh.genVertexTangents();
}


template <typename Gen, typename... Args>
render::MeshData basic(Args&&... args) {
	using namespace render;
	
	MeshData mesh(render::AttrList::Pos3Norm3());

	into<Gen>(mesh, std::forward<Args>(args)...);

	return mesh;
}


// ---- Convenience methods to create a basic MeshData for each of the primitves

template <typename... Args>
render::MeshData plane(Args&&... args) { return basic<Plane>(std::forward<Args>(args)...); }

template <typename... Args>
render::MeshData box(Args&&... args) { return basic<Box>(std::forward<Args>(args)...); }

template <typename... Args>
render::MeshData arc(Args&&... args) { return basic<Arc>(std::forward<Args>(args)...); }

template <typename... Args>
render::MeshData sphere(Args&&... args) { return basic<Sphere>(std::forward<Args>(args)...); }


} // ns gen
} // ns model
} // ns stardazed

#endif
