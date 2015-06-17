// ------------------------------------------------------------------
// system::Time - stardazed
// (c) 2015 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_SYSTEM_TIME_H
#define SD_SYSTEM_TIME_H

#include "system/Config.hpp"

namespace stardazed {
namespace time {


using Time = double;


Time now();
constexpr Time zero() { return 0; }


// create Times of various magnitudes
constexpr Time fromNanoseconds(double ns)  { return ns / 1e9; }
constexpr Time fromMicroseconds(double us) { return us / 1e6; }
constexpr Time fromMilliseconds(double ms) { return ms / 1e4; }
constexpr Time fromSeconds(double s)       { return s; }
constexpr Time fromMinutes(double m)       { return m * 60; }
constexpr Time fromHours(double h)         { return h * 60 * 60; }

constexpr Time hertz(double hz)            { return 1. / hz; }


// convert Times to various magnitudes
constexpr double asNanoseconds(Time t)  { return t * 1e9; }
constexpr double asMicroseconds(Time t) { return t * 1e6; }
constexpr double asMilliseconds(Time t) { return t * 1e4; }
constexpr double asSeconds(Time t)      { return t; }
constexpr double asMinutes(Time t)      { return t / 60.; }
constexpr double asHours(Time t)        { return t / (60. * 60.); }


} // ns time


// -- export Time type in main SD namespace
using Time = time::Time;


} // ns stardazed

#endif
