// ------------------------------------------------------------------
// RenderTarget - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RENDERTARGET_H
#define SD_RENDERTARGET_H

#include <string>
#include <memory>

namespace stardazed {

	class RenderTarget {
		int width, height;
		bool fullscreen;
		
		class Impl;
		std::unique_ptr<Impl> pimpl;

	public:
		RenderTarget(int width, int height, bool fullscreen);
		~RenderTarget();
		
		void swap();
	};
	
}

#endif
