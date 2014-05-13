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

	// The time namespace basically wraps std::chrono specializing on the high_resolution_clock
	// and standardizing on a double precion seconds time value type.

	using Clock    = std::chrono::high_resolution_clock;
	using Duration = std::chrono::duration<double, std::chrono::seconds::period>;
	static_assert(std::chrono::treat_as_floating_point<Duration::rep>::value, "Seconds duration could not be treated as a double");

	inline Duration now() { return Clock::now().time_since_epoch(); }
	inline constexpr Duration zero() { return Duration::zero(); }

	// create arbitrary Durations of a specified length to aid in clarity
	inline constexpr Duration nanoseconds(double ns)  { return std::chrono::duration<double, std::chrono::nanoseconds::period>{ ns }; }
	inline constexpr Duration microseconds(double us) { return std::chrono::duration<double, std::chrono::microseconds::period>{ us }; }
	inline constexpr Duration milliseconds(double ms) { return std::chrono::duration<double, std::chrono::milliseconds::period>{ ms }; }
	inline constexpr Duration seconds(double s)       { return std::chrono::duration<double, std::chrono::seconds::period>{ s }; }
	inline constexpr Duration minutes(double m)       { return std::chrono::duration<double, std::chrono::minutes::period>{ m }; }
	inline constexpr Duration hours(double h)         { return std::chrono::duration<double, std::chrono::hours::period>{ h }; }
	
	inline constexpr Duration hertz(double hz)        { return Duration{ 1. / hz }; }
	
} // ns time
} // ns stardazed

#endif
