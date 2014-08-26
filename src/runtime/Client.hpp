// ------------------------------------------------------------------
// runtime::Client - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RUNTIME_CLIENT_H
#define SD_RUNTIME_CLIENT_H

#include "render/RenderContext.hpp"
#include "audio/AudioContext.hpp"
#include "controls/ControlsContext.hpp"

namespace stardazed {
namespace runtime {


class Client {
	render::RenderContext& renderCtx_;
	audio::AudioContext& audioCtx_;
	controls::ControlsContext& controlsCtx_;
	
public:
	Client(render::RenderContext& rc, audio::AudioContext& ac, controls::ControlsContext& cc)
	: renderCtx_(rc)
	, audioCtx_(ac)
	, controlsCtx_(cc)
	{}
	
	auto& render()   { return renderCtx_; }
	auto& audio()    { return audioCtx_; }
	auto& controls() { return controlsCtx_; }
};


} // ns runtime
} // ns stardazed

#endif
