// ------------------------------------------------------------------
// system::Time - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_SYSTEM_TIME_H
#define SD_SYSTEM_TIME_H

#include <cstdint>
#include <chrono>

namespace stardazed {
namespace time {

	using Clock = std::chrono::high_resolution_clock;
	using Point = Clock::time_point;

	inline Point now() { return Clock::now(); }

} // ns time
} // ns stardazed

#endif
