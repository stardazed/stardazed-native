// ------------------------------------------------------------------
// runtime::Client - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RUNTIME_CLIENT_H
#define SD_RUNTIME_CLIENT_H

#include "render/RenderContext.hpp"
#include "audio/AudioContext.hpp"
#include "event/EventService.hpp"
#include "controls/ControlsContext.hpp"

namespace stardazed {
namespace runtime {


class Client {
	render::RenderContext& renderCtx_;
	audio::AudioContext& audioCtx_;
	event::EventService& eventSvc_;
	
public:
	Client(render::RenderContext& rc, audio::AudioContext& ac, event::EventService& es)
	: renderCtx_(rc)
	, audioCtx_(ac)
	, eventSvc_(es)
	{}
	
	auto& render()   { return renderCtx_; }
	auto& audio()    { return audioCtx_; }
	auto& event()    { return eventSvc_; }
};


} // ns runtime
} // ns stardazed

#endif
