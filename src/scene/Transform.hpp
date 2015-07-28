// ------------------------------------------------------------------
// scene::TransformComponent - stardazed
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
#include "scene/Entity.hpp"

namespace stardazed {
namespace scene {


struct TransformDescriptor {
	math::Vec3 position = math::Vec3::zero();
	math::Quat rotation = math::Quat::identity();
	math::Vec3 scale = math::Vec3::one();
};


class TransformComponent {
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

public:
	TransformComponent();
	
	static const Handle root() { return {0}; }

	// -- array access
	uint32 count() const { return instanceData_.count(); }
	Handle assign(Entity linkedEntity, const Handle parent = root());
	Handle assign(Entity linkedEntity, const TransformDescriptor&, const Handle parent = root());

	// -- single instance data access
	Handle parent(Handle h) const { return parentBase_[h.ref]; }
	const math::Vec3& position(Handle h) const { return positionBase_[h.ref]; }
	const math::Quat& rotation(Handle h) const { return rotationBase_[h.ref]; }
	const math::Vec3& scale(Handle h) const { return scaleBase_[h.ref]; }
	const math::Mat4& modelMatrix(Handle h) const { return modelMatrixBase_[h.ref]; }

	void setParent(const Handle, const Handle newParent);
	void setPosition(const Handle, const math::Vec3&);
	void setRotation(const Handle, const math::Quat&);
	void setPositionAndRotation(const Handle, const math::Vec3&, const math::Quat&);
	void setScale(const Handle h, const math::Vec3& newScale);

	// -- single instance state modifiers
	Handle forEntity(Entity) const;
	
	void rotate(const Handle h, math::Angle overX, math::Angle overY, math::Angle overZ) {
		setRotation(h, rotation(h) * math::Quat::fromEuler(overZ, overY, overX));
	}
	
	void rotate(const Handle h, const math::Vec3& axis, math::Angle angle) {
		setRotation(h, rotation(h) * math::Quat::fromAxisAngle(axis, angle));
	}
	
	void rotate(const Handle h, const math::Quat& q) {
		setRotation(h, rotation(h) * q);
	}
	
	void translate(const Handle h, const math::Vec3& translation) {
		float len = math::length(translation);
		if (! math::nearEqual(len, 0.0f))
			setPosition(h, position(h) + (rotation(h) * translation) * len);
	}
	
	void translate(const Handle h, float x, float y, float z) {
		translate(h, math::Vec3{ x, y, z });
	}
	
	void translateGlobal(const Handle h, const math::Vec3& globalTranslation) {
		setPosition(h, position(h) + globalTranslation);
	}
	
	void translateGlobal(const Handle h, float gx, float gy, float gz) {
		setPosition(h, position(h) + math::Vec3{ gx, gy, gz });
	}

	void lookAt(const Handle h, const math::Vec3& target, const math::Vec3& up);
};


} // ns scene
} // ns stardazed

#endif
