// ------------------------------------------------------------------
// render::opengl::Buffer - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "render/opengl/Buffer.hpp"
#include "render/common/VertexBuffer.hpp"

namespace stardazed {
namespace render {
		

namespace detail {

// internal buffer options to gl options
constexpr GLbitfield glAccessFlagsForBCA(BufferClientAccess access) {
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


constexpr GLenum glUsageHint(BufferUpdateFrequency frequency, BufferClientAccess typicalAccess) {
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


constexpr GLenum glTargetForArrayType(GLArrayType type) {
	switch (type) {
		case GLArrayType::Attribute:
			return GL_ARRAY_BUFFER;
		case GLArrayType::Index:
			return GL_ELEMENT_ARRAY_BUFFER;
	}

	assert(false && "Invalid array type");
}


constexpr GLenum glBindingNameForTarget(GLenum target) {
	switch (target) {
		case GL_ARRAY_BUFFER:              return GL_ARRAY_BUFFER_BINDING;
		case GL_ELEMENT_ARRAY_BUFFER:      return GL_ELEMENT_ARRAY_BUFFER_BINDING;
		case GL_UNIFORM_BUFFER:            return GL_UNIFORM_BUFFER_BINDING;
		case GL_TRANSFORM_FEEDBACK_BUFFER: return GL_TRANSFORM_FEEDBACK_BUFFER_BINDING;
		case GL_PIXEL_PACK_BUFFER:         return GL_PIXEL_PACK_BUFFER_BINDING;
		case GL_PIXEL_UNPACK_BUFFER:       return GL_PIXEL_UNPACK_BUFFER;
		case GL_TEXTURE_BUFFER:            return GL_TEXTURE_BINDING_BUFFER;
//		case GL_COPY_READ_BUFFER:          return GL_COPY_READ_BUFFER_BINDING;   not available in gl3.h?
//		case GL_COPY_WRITE_BUFFER:         return GL_COPY_WRITE_BUFFER_BINDING;
		default:
			assert(false && "Unknown buffer target name");
	}
}


} // ns detail


} // ns render
} // ns stardazed
