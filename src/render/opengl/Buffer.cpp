// ------------------------------------------------------------------
// render::opengl::Buffer - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "render/opengl/Buffer.hpp"
#include "render/common/VertexBuffer.hpp"

namespace stardazed {
namespace render {
		

namespace detail {
	template <GLenum target>
	GLint IndexedBufferOps<target>::maxIndex_ = -1;
}


namespace ConstantBufferLimits {

	const MaxComponents& maxComponents() {
		static MaxComponents maxComponents_s {};

		if (maxComponents_s.vertexStage == 0) {
			GLint maxVert, maxGeom, maxFrag;

			glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &maxVert);
			glGetIntegerv(GL_MAX_GEOMETRY_UNIFORM_COMPONENTS, &maxGeom);
			glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, &maxFrag);

			maxComponents_s.vertexStage = maxVert;
			maxComponents_s.geometryStage = maxGeom;
			maxComponents_s.fragmentStage = maxFrag;
			maxComponents_s.allStages = math::min(math::min(maxVert, maxGeom), maxFrag);
		}
		
		return maxComponents_s;
	}


	uint32 maxBlockSize() {
		static uint32 maxBlockSize_s = 0;
		
		if (maxBlockSize_s == 0) {
			GLint maxBlock;
			glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &maxBlock);
			maxBlockSize_s = maxBlock;
		}

		return maxBlockSize_s;
	}


	uint32 offsetAlignment() {
		static uint32 offsetAlignment_s = 0;
		
		if (offsetAlignment_s == 0) {
			GLint offsetAlignment;
			glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &offsetAlignment);
			offsetAlignment_s = offsetAlignment;
		}
		
		return offsetAlignment_s;
	}
	
} // ns ConstantBufferLimits


} // ns render
} // ns stardazed
