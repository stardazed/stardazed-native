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


enum class NodeType {
	Generic,
	Light,
	Camera
};


class Node {
	NodeType type_;

public:
	Node(NodeType type) : type_(type) {}
	Node() : Node(NodeType::Generic) {}
	
	NodeType type() const { return type_; }

	// -- node components
	Transform transform;
	render::Mesh* mesh;
	render::Material* material;
};


} // ns scene
} // ns stardazed

#endif
