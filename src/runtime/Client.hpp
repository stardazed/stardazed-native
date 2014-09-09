// ------------------------------------------------------------------
// runtime::Client - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RUNTIME_CLIENT_H
#define SD_RUNTIME_CLIENT_H

#include "render/RenderContext.hpp"
#include "audio/AudioContext.hpp"
#include "device/DevicesContext.hpp"
#include "runtime/TimeContext.hpp"

namespace stardazed {
namespace runtime {


class Client {
	render::RenderContext& renderCtx_;
	audio::AudioContext& audioCtx_;
	device::DevicesContext& devicesCtx_;
	TimeContext timeCtx_;
	
public:
	Client(
	   render::RenderContext& rc, audio::AudioContext& ac,
	   device::DevicesContext& dc
	)
	: renderCtx_(rc)
	, audioCtx_(ac)
	, devicesCtx_(dc)
	{}
	
	auto& render()   { return renderCtx_; }
	auto& audio()    { return audioCtx_; }
	auto& devices()  { return devicesCtx_; }
	auto& time()     { return timeCtx_; }
};


} // ns runtime
} // ns stardazed

#endif
