// ------------------------------------------------------------------
// controls::Bindings - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_CONTROLS_BINDINGS_H
#define SD_CONTROLS_BINDINGS_H

#include "device/DevicesContext.hpp"

namespace stardazed {
namespace controls {


class Key {
	device::Key key_;

public:
	constexpr Key(device::Key k)
	: key_(k)
	{}

	float value(const DevicesSnapshot& dss) const {
		return dss.keyboard().isPressed(key_) ? 1 : 0;
	}
};


class KeyPair {
	device::Key keyA_, keyB_;

public:
	constexpr KeyPair(device::Key a, device::Key b)
	: keyA_(a)
	, keyB_(b)
	{}
	
	float value(const DevicesSnapshot& dss) const {
		float a = dss.keyboard().isPressed(keyA_) ? -1 : 0,
			  b = dss.keyboard().isPressed(keyB_) ?  1 : 0;
		
		return a + b;
	}
};


} // ns controls
} // ns stardazed

#endif
