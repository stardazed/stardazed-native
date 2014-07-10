// ------------------------------------------------------------------
// scene::Scene - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

namespace stardazed {
namespace scene {


class Node {
public:
	Transform transform;

	render::Mesh* mesh;
	render::Material* material;
};



class Scene {
public:
	Node* makeNode();
	Camera* makeCamera();

private:
	
};


} // ns scene
} // ns stardazed

#endif
