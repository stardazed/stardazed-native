// ------------------------------------------------------------------
// system::RenderTarget - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_SYSTEM_RENDERTARGET_H
#define SD_SYSTEM_RENDERTARGET_H

#include "RenderTargetOptions.hpp"
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


} // ns stardazed

#endif
