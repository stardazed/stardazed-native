// ------------------------------------------------------------------
// scene::Camera - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "scene/Camera.hpp"

namespace stardazed {
namespace scene {


namespace detail {
	using namespace math;

	template <typename T>
	Matrix<4, 4, T> perspective(Angle fovy, T aspect, T zNear, T zFar) {
		auto tanHalfFovy = std::tan(fovy.rad().val() / T{2} );
		
		Matrix<4, 4, T> result;
		result[0][0] = T{1} / (aspect * tanHalfFovy);
		result[1][1] = T{1} / tanHalfFovy;
		result[2][2] = -(zFar + zNear) / (zFar - zNear);
		result[2][3] = -1;
		result[3][2] = -(T{2} * zFar * zNear) / (zFar - zNear);
		return result;
	}
}


Camera::Camera(Node& linkedNode)
: node_(&linkedNode)
{
}


void Camera::perspective(math::Angle fovy, float aspect, float zNear, float zFar) {
	projection_ = detail::perspective(fovy, aspect, zNear, zFar);
}


void Camera::perspective(math::Angle fovy, size_t pixelWidth, size_t pixelHeight, float zNear, float zFar) {
	float aspect = static_cast<float>(pixelWidth) / static_cast<float>(pixelHeight);
	perspective(fovy, aspect, zNear, zFar);
}


const math::Mat4 Camera::viewMatrix() const {
	using namespace math;
	
	auto& trans = transform();
	auto m = inverse(trans.rotation).toMatrix4();
	
	// translation
    m[3].xyz = {
		dot(Vec3{m[0][0], m[1][0], m[2][0]}, trans.position),
		dot(Vec3{m[0][1], m[1][1], m[2][1]}, trans.position),
		dot(Vec3{m[0][2], m[1][2], m[2][2]}, trans.position)
	};

	return m;
}



} // ns scene
} // ns stardazed
