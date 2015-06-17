// ------------------------------------------------------------------
// runtime::FrameContext - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RUNTIME_FRAMECONTEXT_H
#define SD_RUNTIME_FRAMECONTEXT_H

#include "runtime/Client.hpp"
#include "device/DevicesContext.hpp"
#include "system/Time.hpp"

namespace stardazed {
namespace runtime {


class FrameContext {
	Client& client_;
	Time deltaTime_;

public:
	FrameContext(Client& client,
				 Time deltaTime)
	: client_(client)
	, deltaTime_(deltaTime)
	{}
	
	Time deltaTime() { return deltaTime_; }

	auto& client()  { return client_; }
};


} // ns runtime
} // ns stardazed

#endif
