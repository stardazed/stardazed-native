// ------------------------------------------------------------------
// render::IndexBuffer - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDER_INDEXBUFFER_H
#define SD_RENDER_INDEXBUFFER_H

#include "system/Config.hpp"
#include "render/common/BufferStorage.hpp"

#include <memory>

namespace stardazed {
namespace render {


enum class VertexWinding {
	Clockwise,
	CounterClockwise
};


class IndexBuffer {
	std::unique_ptr<BufferStorage> storage_;
	VertexWinding winding_;
	
	void reverseWinding();

public:
	IndexBuffer(VertexWinding winding = VertexWinding::CounterClockwise)
	: winding_{winding}
	{}
	
	// --
	
	void allocate(size32_t triangleCount);
	
	// --
	
	VertexWinding winding() const { return winding_; }
	
	void setWinding(VertexWinding newWinding) {
		if (newWinding == winding_)
			return;
		winding_ = newWinding;
		reverseWinding();
	}

	void flipWinding() {
		if (winding_ == VertexWinding::Clockwise)
			setWinding(VertexWinding::CounterClockwise);
		else
			setWinding(VertexWinding::Clockwise);
	}
};


} // ns render
} // ns stardazed

#endif
