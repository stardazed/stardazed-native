// ------------------------------------------------------------------
// system::RenderContext - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_SYSTEM_RENDERCONTEXT_H
#define SD_SYSTEM_RENDERCONTEXT_H

#include "RenderContextOptions.hpp"
#include <string>
#include <memory>

namespace stardazed {


class RenderContext {
	RenderContextOptions options;
	
	class Impl;
	std::unique_ptr<Impl> pimpl;

public:
	RenderContext(RenderContextOptions options);
	~RenderContext();
	
	void swap();
};


} // ns stardazed

#endif
