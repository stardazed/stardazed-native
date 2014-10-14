// ------------------------------------------------------------------
// render::opengl::Buffer - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "render/opengl/Buffer.hpp"

namespace stardazed {
namespace render {
		

namespace detail {
	// set the proper default pointer for a Buffer for a contained type T
	
	template <>
	void setDefaultAttribPointer<math::Vec2>(GLuint attribIndex) {
		glVertexAttribPointer(attribIndex, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	}
	
	template <>
	void setDefaultAttribPointer<math::Vec3>(GLuint attribIndex) {
		glVertexAttribPointer(attribIndex, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	}
	
	template <>
	void setDefaultAttribPointer<math::Vec4>(GLuint attribIndex) {
		glVertexAttribPointer(attribIndex, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
	}
	
	template <>
	void setDefaultAttribPointer<render::Tri16>(GLuint attribIndex) {
		glVertexAttribPointer(attribIndex, 3, GL_UNSIGNED_SHORT, GL_FALSE, 0, nullptr);
	}
	
	template <>
	void setDefaultAttribPointer<render::Tri32>(GLuint attribIndex) {
		glVertexAttribPointer(attribIndex, 3, GL_UNSIGNED_INT, GL_FALSE, 0, nullptr);
	}

} // ns detail


} // ns render
} // ns stardazed
