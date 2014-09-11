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
	device::DevicesSnapshot devicesSnapshot_;
	time::Duration deltaTime_;

public:
	FrameContext(Client& client,
				 time::Duration deltaTime)
	: client_(client)
	, devicesSnapshot_(client_.devices().snapshot())
	, deltaTime_(deltaTime)
	{}
	
	time::Duration deltaTime() { return deltaTime_; }

	auto& client()  { return client_; }
	auto& input()   { return devicesSnapshot_; }
};


} // ns runtime
} // ns stardazed

#endif
