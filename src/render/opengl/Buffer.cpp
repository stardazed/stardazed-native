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


	// internal buffer options to gl options
	GLbitfield glAccessFlagsForBCA(BufferClientAccess access) {
		switch (access) {
			case BufferClientAccess::ReadOnly:
				return GL_MAP_READ_BIT;
			case BufferClientAccess::WriteOnly:
				return GL_MAP_WRITE_BIT;
			case BufferClientAccess::ReadWrite:
				return GL_MAP_READ_BIT | GL_MAP_WRITE_BIT;
			default:
				return 0;
		}
	}
	
	GLenum glUsageHint(BufferUpdateFrequency frequency, BufferClientAccess typicalAccess) {
		if (typicalAccess == BufferClientAccess::None) {
			switch (frequency) {
				case BufferUpdateFrequency::Never:
					return GL_STATIC_COPY;
				case BufferUpdateFrequency::Occassionally:
					return GL_DYNAMIC_COPY;
				case BufferUpdateFrequency::Frequently:
					return GL_STREAM_COPY;
			}
		}
		
		if (typicalAccess == BufferClientAccess::ReadWrite || typicalAccess == BufferClientAccess::WriteOnly) {
			switch (frequency) {
				case BufferUpdateFrequency::Never:
					return GL_STATIC_DRAW;
				case BufferUpdateFrequency::Occassionally:
					return GL_DYNAMIC_DRAW;
				case BufferUpdateFrequency::Frequently:
					return GL_STREAM_DRAW;
			}
		}
		
		if (typicalAccess == BufferClientAccess::ReadOnly) {
			switch (frequency) {
				case BufferUpdateFrequency::Never:
					return GL_STATIC_READ;
				case BufferUpdateFrequency::Occassionally:
					return GL_DYNAMIC_READ;
				case BufferUpdateFrequency::Frequently:
					return GL_STREAM_READ;
			}
		}
		
		assert(false && "Invalid frequency or access level");
	}
	
	GLenum glTargetForArrayType(GLArrayType type) {
		switch (type) {
			case GLArrayType::Attribute:
				return GL_ARRAY_BUFFER;
			case GLArrayType::Index:
				return GL_ELEMENT_ARRAY_BUFFER;
		}

		assert(false && "Invalid array type");
	}

} // ns detail


} // ns render
} // ns stardazed
