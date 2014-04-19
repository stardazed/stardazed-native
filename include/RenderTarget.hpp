// ------------------------------------------------------------------
// RenderTarget - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDERTARGET_H
#define SD_RENDERTARGET_H

#include "RenderTargetOptions.h"
#include <string>
#include <memory>

namespace stardazed {

class RenderTarget {
	RenderTargetOptions options;
	
	class Impl;
	std::unique_ptr<Impl> pimpl;

public:
	RenderTarget(RenderTargetOptions options);
	~RenderTarget();
	
	void swap();
};
	
} // stardazed namespace

#endif
