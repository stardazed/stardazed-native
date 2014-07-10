// ------------------------------------------------------------------
// sim::Scene - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

namespace stardazed {



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
	Light* makeLight;
	

private:
	
};


} // ns stardazed

#endif
