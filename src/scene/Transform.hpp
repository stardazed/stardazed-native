// ------------------------------------------------------------------
// scene::Transform - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_SCENE_TRANSFORM_H
#define SD_SCENE_TRANSFORM_H

#include "system/Config.hpp"
#include "math/Angle.hpp"
#include "math/Vector.hpp"
#include "math/Matrix.hpp"
#include "math/Quaternion.hpp"
#include "container/MultiElementArrayBuffer.hpp"

namespace stardazed {
namespace scene {


class Transform {
public:
	struct Handle { uint32 ref; };
	
private:
	container::MultiElementArrayBuffer<
		Handle,     // parentHandle
		math::Vec3, // position
		math::Quat, // rotation
		math::Vec3, // scale
		math::Mat4  // modelMatrix
	> instanceData_;
	
	Handle* parentBase_;
	math::Vec3* positionBase_;
	math::Quat* rotationBase_;
	math::Vec3* scaleBase_;
	math::Mat4* modelMatrixBase_;
	
	void rebase();
	
	uint32 nextRef_ = 1;

public:
	Transform();

	// -- shared Component `interface`
	uint32 count() const { return instanceData_.count(); }
	Handle append(Handle parent);

	// -- single instance data access
	Handle parent(Handle h) const { return parentBase_[h.ref]; }
	const math::Vec3& position(Handle h) const { return positionBase_[h.ref]; }
	const math::Quat& rotation(Handle h) const { return rotationBase_[h.ref]; }
	const math::Vec3& scale(Handle h) const { return scaleBase_[h.ref]; }
	const math::Mat4& modelMatrix(Handle h) const { return modelMatrixBase_[h.ref]; }

	void setParent(Handle h, Handle newParent);
	void setPosition(Handle h, const math::Vec3& newPosition);
	void setRotation(Handle h, const math::Quat& newRotation);
	void setScale(Handle h, const math::Vec3& newScale);
};


struct TransformX {
	math::Vec3 position;
	math::Vec3 scale { 1, 1, 1 };
	math::Quat rotation;

	void lookAt(const math::Vec3& target, const math::Vec3& up = {0,1,0});

	void rotate(math::Angle overX, math::Angle overY, math::Angle overZ) {
		rotation *= math::Quat::fromEuler(overZ, overY, overX);
	}

	void rotate(const math::Vec3& axis, math::Angle angle) {
		rotation *= math::Quat::fromAxisAngle(axis, angle);
	}

	void rotate(const math::Quat& q) {
		rotation *= q;
	}
	
	void translate(const math::Vec3& translation) {
		float len = math::length(translation);
		if (math::nearEqual(len, 0.0f))
			return;
		position += (rotation * translation) * len;
	}

	void translate(float x, float y, float z) {
		translate(math::Vec3{ x, y, z });
	}

	void translateGlobal(const math::Vec3& globalTranslation) {
		position += globalTranslation;
	}

	void translateGlobal(float gx, float gy, float gz) {
		position += math::Vec3{ gx, gy, gz };
	}
	
	Transform apply(const Transform&) const;

	math::Mat4 toMatrix4() const;
};


} // ns scene
} // ns stardazed

#endif
