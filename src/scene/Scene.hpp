// ------------------------------------------------------------------
// scene::Scene - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "scene/Node.hpp"

namespace stardazed {
namespace scene {


class Scene {
public:
	Node* makeNode();
	Camera* makeCamera();

private:
	
};


} // ns scene
} // ns stardazed

#endif
