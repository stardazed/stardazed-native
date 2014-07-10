// ------------------------------------------------------------------
// scene::Scene - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

namespace stardazed {
namespace scene {


class Node {
public:
	std::string name;
	Transform transform;

	Mesh* mesh;
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
