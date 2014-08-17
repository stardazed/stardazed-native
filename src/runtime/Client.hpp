// ------------------------------------------------------------------
// runtime::Client - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RUNTIME_CLIENT_H
#define SD_RUNTIME_CLIENT_H

#include "render/RenderContext.hpp"
#include "audio/AudioContext.hpp"
#include "input/InputContext.hpp"

namespace stardazed {
namespace runtime {


class Client {
	render::RenderContext& renderCtx_;
	audio::AudioContext& audioCtx_;
	input::InputContext& inputCtx_;
	
public:
	Client(render::RenderContext& rc, audio::AudioContext& ac, input::InputContext& ic)
	: renderCtx_(rc)
	, audioCtx_(ac)
	, inputCtx_(ic)
	{}
	
	auto& render() { return renderCtx_; }
	auto& audio()  { return audioCtx_; }
	auto& input()  { return inputCtx_; }
};


} // ns runtime
} // ns stardazed

#endif
