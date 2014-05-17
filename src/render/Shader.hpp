// ------------------------------------------------------------------
// render::Shader - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_SHADER_H
#define SD_RENDER_SHADER_H

#include <string>
#include "render/OpenGL.hpp"


namespace stardazed {
namespace render {


class Shader {
	GLuint program_;
	GLuint mvMatrix_, mvpMatrix_, normalMatrix_;
	
public:
	Shader(GLenum type, const std::string& source);
	~Shader();
	
	GLuint program() const { return program_; }
	GLuint mvMatrix() const { return mvMatrix_; }
	GLuint mvpMatrix() const { return mvpMatrix_; }
	GLuint normalMatrix() const { return normalMatrix_; }
};


Shader makeShaderWithPath(GLenum type, const std::string& filePath);
GLuint makeSimplePipeline(const Shader& vertexShader, const Shader& fragmentShader);


} // ns render
} // ns stardazed

#endif
