// ------------------------------------------------------------------
// controls::Bindings - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_CONTROLS_BINDINGS_H
#define SD_CONTROLS_BINDINGS_H

#include "math/Vector.hpp"
#include "controls/ControlsContext.hpp"

namespace stardazed {
namespace controls {


class Key {
	event::Key key_;
	float value_ = 0;

public:
	constexpr Key(event::Key k)
	: key_(k)
	{}

	void update(ControlsContext& controls) {
		value_ = controls.isKeyPressed(key_) ? 1 : 0;
	}
	
	constexpr float value() const { return value_; }
};


class KeyPair {
	event::Key keyA_, keyB_;
	float value_ = 0;

public:
	constexpr Key(event::Key a, event::Key b)
	: keyA_(a)
	, keyB_(b)
	{}
	
	void update(ControlsContext& controls) {
		float a = controls.isKeyPressed(keyA_) ? -1 : 0,
			  b = controls.isKeyPressed(keyB_) ?  1 : 0;
		
		value_ = a + b;
	}
	
	constexpr float value() const { return value_; }
};



} // ns controls
} // ns stardazed

#endif
