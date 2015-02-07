// ------------------------------------------------------------------
// render::opengl::OpenGL.cpp - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#include "render/opengl/OpenGL.hpp"
#include "system/Logging.hpp"

#include <string>
#include <unordered_set>

namespace stardazed {
namespace render {


static const std::unordered_set<std::string>& glExtensionSet() {
	static std::unordered_set<std::string> extensionSet;
	
	if (extensionSet.size() == 0) {
		GLint extCount = 0;
		glGetIntegerv(GL_NUM_EXTENSIONS, &extCount);
		for (auto i=0; i < extCount; ++i)
			extensionSet.insert((const char*)glGetStringi(GL_EXTENSIONS, i));
	}
	
	return extensionSet;
}
	

bool OpenGL::hasExtension(const char* extensionName) {
	auto extensions = glExtensionSet();
	return extensions.find(extensionName) != extensions.end();
}


} // ns render
} // ns stardazed
