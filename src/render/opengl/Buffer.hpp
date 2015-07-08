// ------------------------------------------------------------------
// render::opengl::Buffer - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_OPENGL_BUFFER_H
#define SD_RENDER_OPENGL_BUFFER_H

#include "system/Config.hpp"
#include "util/ConceptTraits.hpp"

#include "math/Vector.hpp"
#include "render/common/Buffer.hpp"
#include "render/common/Mesh.hpp"
#include "render/opengl/OpenGL.hpp"


namespace stardazed {
namespace render {


namespace detail {
	// internal buffer options to gl options
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
		
		assert(!"Invalid frequency or access level");
		return GL_NONE;
	}
	
	
	constexpr GLenum glTargetForBufferRole(BufferRole role) {
		switch (role) {
			case BufferRole::VertexAttribute:
				return GL_ARRAY_BUFFER;
			case BufferRole::VertexIndex:
				return GL_ELEMENT_ARRAY_BUFFER;
			case BufferRole::TextureBuffer:
				return GL_TEXTURE_BUFFER;
			case BufferRole::TransformFeedback:
				return GL_TRANSFORM_FEEDBACK_BUFFER;
			case BufferRole::ConstantBuffer:
				return GL_UNIFORM_BUFFER;
		}
	}
	
	
	constexpr GLenum glBindingNameForTarget(GLenum target) {
		switch (target) {
			case GL_ARRAY_BUFFER:              return GL_ARRAY_BUFFER_BINDING;
			case GL_ELEMENT_ARRAY_BUFFER:      return GL_ELEMENT_ARRAY_BUFFER_BINDING;
			case GL_TEXTURE_BUFFER:            return GL_TEXTURE_BINDING_BUFFER;
			case GL_TRANSFORM_FEEDBACK_BUFFER: return GL_TRANSFORM_FEEDBACK_BUFFER_BINDING;
			case GL_UNIFORM_BUFFER:            return GL_UNIFORM_BUFFER_BINDING;
			default:
				assert(!"Unhandled buffer target name");
				return GL_NONE;
		}
	}
}


class Buffer {
	BufferRole role_;
	BufferUpdateFrequency updateFrequency_;
	BufferClientAccess clientAccess_;
	GLenum target_ {0};
	GLResource resource_ {};
	size32 byteSize_ {0};

public:
	Buffer(BufferRole role, BufferUpdateFrequency frequency, BufferClientAccess access)
	: role_(role)
	, updateFrequency_(frequency)
	, clientAccess_(access)
	, target_{ detail::glTargetForBufferRole(role) }
	{
		GLuint bufferName = 0;
		glGenBuffers(1, &bufferName);
		resource_.assign(bufferName);
	}
	
	~Buffer() {
		auto bufferName = name();
		if (bufferName) {
			glDeleteBuffers(1, &bufferName);
			resource_.clear();
		}
	}
	
	SD_DEFAULT_MOVE_OPS(Buffer)

	// -- initialization

	void allocate(size32 bytes, void* data) {
		byteSize_ = bytes;
		bind();
		glBufferData(target_, bytes, data, detail::glUsageHint(updateFrequency(), clientAccess()));
	}
	
	void allocate(size32 bytes) {
		allocate(bytes, nullptr);
	}
	
	void allocateFromVertexBuffer(const VertexBuffer& vb) {
		assert(role() == BufferRole::VertexAttribute);
		allocate(vb.bufferSizeBytes(), vb.basePointer());
	}
	
	void allocateFromIndexBuffer(const IndexBuffer& ib) {
		assert(role() == BufferRole::VertexIndex);
		allocate(ib.bufferSizeBytes(), ib.basePointer());
	}

	// -- direct updates
	
	void write(size32 bytes, const void* data, size32 offset) {
		bind();
		glBufferSubData(target_, offset, bytes, data);
	}

	// -- memory mapped access

private:
	template <typename T>
	T* mapForUpdates(size32 offset, size32 bytes, GLbitfield flags) {
		assert(offset + bytes < byteSize_);
		bind();
		return static_cast<T*>(glMapBufferRange(target_, offset, bytes, flags));
	}

public:
	template <typename T>
	const T* mapRangeForReading(size32 offset, size32 bytes) {
		assert(offset + bytes < byteSize_);
		bind();
		return static_cast<const T*>(glMapBufferRange(target_, offset, bytes, GL_MAP_READ_BIT));
	}
	
	template <typename T>
	const T* mapBufferForReading() {
		return mapRangeForReading<T>(0, byteSize_);
	}
	
	template <typename T>
	T* mapRangeForWriting(size32 offset, size32 bytes) {
		return mapForUpdates<T>(offset, bytes, GL_MAP_WRITE_BIT);
	}
	
	template <typename T>
	T* mapBufferForWriting() {
		return mapRangeForWriting<T>(0, byteSize_);
	}
	
	template <typename T>
	T* invalidateAndMapRangeForWriting(size32 offset, size32 bytes) {
		return mapForUpdates<T>(offset, bytes, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT);
	}
	
	template <typename T>
	T* invalidateAndMapBufferForWriting() {
		return mapForUpdates<T>(0, byteSize_, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
	}

	template <typename T>
	T* mapRangeForFullAccess(size32 offset, size32 bytes) {
		return mapForUpdates<T>(offset, bytes, GL_MAP_READ_BIT | GL_MAP_WRITE_BIT);
	}
	
	template <typename T>
	T* mapBufferForFullAccess() {
		return mapRangeForFullAccess<T>(0, byteSize_);
	}
	
	void unmap() {
		glUnmapBuffer(target_);
	}
	
	// -- observers
	BufferRole role() const { return role_; }
	BufferUpdateFrequency updateFrequency() const { return updateFrequency_; }
	BufferClientAccess clientAccess() const { return clientAccess_; }
	size32 byteSize() const { return byteSize_; }

	GLuint name() const { return resource_.name(); }
	GLenum target() const { return target_; }
	
	// -- binding
	
	void bind() const { glBindBuffer(target_, name()); }
	
	static GLuint boundAtTarget(GLenum target) {
		GLuint currentlyBound;
		glGetIntegerv(detail::glBindingNameForTarget(target), reinterpret_cast<GLint*>(&currentlyBound));
		return currentlyBound;
	}
};


// ---- Indexed Buffer Targets

namespace detail {

	constexpr GLenum maxBufferBindingsNameForTarget(GLenum target) {
		if (target == GL_UNIFORM_BUFFER)
			return GL_MAX_UNIFORM_BUFFER_BINDINGS;
		if (target == GL_TRANSFORM_FEEDBACK_BUFFER)
			return GL_MAX_TRANSFORM_FEEDBACK_BUFFERS;

		assert(!"Unknown indexed buffer target");
		return GL_NONE;
	}

	template <GLenum target>
	class IndexedBufferOps {
		static_assert(target == GL_UNIFORM_BUFFER || target == GL_TRANSFORM_FEEDBACK_BUFFER, "Invalid index buffer target");
		IndexedBufferOps() {}
		
		static GLint maxIndex_;

	public:
		static GLint maxIndex() {
			if (maxIndex_ < 0) {
				glGetIntegerv(maxBufferBindingsNameForTarget(target), &maxIndex_);
			}
			return maxIndex_;
		}

		static void bindBufferToBindPoint(const Buffer& buffer, uint32 bindPoint) {
			assert(static_cast<GLint>(bindPoint) < maxIndex());
			glBindBufferBase(target, bindPoint, buffer.name());
		}

		static void bindBufferRangeToBindPoint(const Buffer& buffer, size32 offset, size32 bytes, uint32 bindPoint) {
			assert(static_cast<GLint>(bindPoint) < maxIndex());
			assert(offset + bytes <= buffer.byteSize());
			glBindBufferRange(target, bindPoint, buffer.name(), static_cast<GLintptr>(offset), bytes);
		}
	};

} // ns detail


// -- GL indexed uniform blocks
template class detail::IndexedBufferOps<GL_UNIFORM_BUFFER>;
using IndexedUniformBlocks = detail::IndexedBufferOps<GL_UNIFORM_BUFFER>;

// -- GL indexed transform feedback blocks
template class detail::IndexedBufferOps<GL_TRANSFORM_FEEDBACK_BUFFER>;
using IndexedTransformFeedbackBlocks = detail::IndexedBufferOps<GL_TRANSFORM_FEEDBACK_BUFFER>;



// ---- Constant Buffer Limits

namespace ConstantBufferLimits {
	struct MaxComponents {
		uint32 vertexStage = 0;
		uint32 geometryStage = 0;
		uint32 fragmentStage = 0;
		
		uint32 allStages = 0;
	};
	
	const MaxComponents& maxComponents(); // maximum # of individually accessible components in a buffer
	uint32 maxBlockSize();                // maximum bindable size of buffer
	uint32 offsetAlignment();             // required alignment of offsets for BindRange, etc.
	
	template <typename T>
	uint32 maximumAccessibleArrayElementsPerBlock() {
		auto& componentLimits = ConstantBufferLimits::maxComponents();
		auto maxUsableComponents = componentLimits.allStages;
		auto componentsPerElement = math::max(1u, sizeof32<T>() / sizeof32<float>());
		auto maxComponentLimitedElements = maxUsableComponents / componentsPerElement;

		// -- WARNING: this does not take into account any <float sized components in struct T
		auto maxConstBlockSize = ConstantBufferLimits::maxBlockSize();
		auto maxBlockSizeLimitedElements = maxConstBlockSize / sizeof32<T>();
		
		return math::min(maxBlockSizeLimitedElements, maxComponentLimitedElements);
	}
}


} // ns render
} // ns stardazed

#endif
