// ------------------------------------------------------------------
// system::mac_Time.cpp - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#include "system/Time.hpp"
#include <mach/mach_time.h>

namespace stardazed {
namespace time {


Time now() {
	uint64 absTime = mach_absolute_time();

	static mach_timebase_info_data_t timebaseInfo {};
	if (__builtin_expect(timebaseInfo.denom == 0, 0)) {
		mach_timebase_info(&timebaseInfo);
	}
	
	uint64 elapsedNano = absTime * timebaseInfo.numer / timebaseInfo.denom;
	return Time(elapsedNano) / 1e9;
}


} // ns scene
} // ns stardazed
