// ------------------------------------------------------------------
// scene::Node - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_SCENE_NODE_H
#define SD_SCENE_NODE_H

#include "scene/Transform.hpp"
#include "render/Mesh.hpp"
#include "render/Material.hpp"

namespace stardazed {
namespace scene {


class Node {
	Transform transform_;

public:
	Transform& transform() { return transform_; }
	const Transform& transform() const { return transform_; }
	
	render::Mesh* mesh;
	render::Material* material;
};


} // ns scene
} // ns stardazed

#endif
