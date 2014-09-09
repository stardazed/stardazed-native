// ------------------------------------------------------------------
// runtime::TimeContext - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_RUNTIME_TIMECONTEXT_H
#define SD_RUNTIME_TIMECONTEXT_H

#include "system/Time.hpp"

namespace stardazed {
namespace runtime {


class TimeContext {
	time::Duration minRenderFrameTime_;
	time::Duration previousTime_;

public:
	TimeContext();
	
	void setMaximumFPS(float);
	
	void step();
	
	time::Duration delta() const;
};


} // ns runtime
} // ns stardazed

#endif
